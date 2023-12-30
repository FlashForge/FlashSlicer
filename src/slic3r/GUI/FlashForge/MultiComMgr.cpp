#include "MultiComMgr.hpp"
#include "FreeInDestructor.h"

namespace Slic3r { namespace GUI {

MultiComMgr::MultiComMgr()
    : m_idNum(0)
{
    m_datMap.emplace(ComInvalidId, com_dev_data_t());
}

bool MultiComMgr::initalize(const std::string &newtworkDllPath)
{
    if (networkIntfc() != nullptr) {
        return false;
    }
    m_networkIntfc.reset(new fnet::FlashNetworkIntfc(newtworkDllPath.c_str()));
    if (!m_networkIntfc->isOk()) {
        m_networkIntfc.reset(nullptr);
        return false;
    }
    m_wanDevUpdateThd.reset(new WanDevUpdateThd(m_networkIntfc.get()));
    m_wanDevUpdateThd->Bind(WAN_DEV_UPDATE_EVENT, &MultiComMgr::onWanDevUpdated, this);
    return true;
}

void MultiComMgr::uninitalize()
{
    m_wanDevUpdateThd->exit();
    m_wanDevUpdateThd.reset(nullptr);
    m_networkIntfc.reset(nullptr);
}

fnet::FlashNetworkIntfc *MultiComMgr::networkIntfc()
{
    return m_networkIntfc.get();
}

void MultiComMgr::addLanDev(const fnet_lan_dev_info &devInfo, const std::string &checkCode)
{
    initConnection(com_ptr_t(new ComConnection(m_idNum++, checkCode, devInfo, m_networkIntfc.get())));
}

void MultiComMgr::setWanDevToken(const std::string &accessToken)
{
    m_wanDevUpdateThd->setToken(accessToken);
}

void MultiComMgr::removeWanDev()
{
    m_wanDevUpdateThd->setToken("");
    for (auto &comPtr : m_comPtrs) {
        comPtr.get()->disconnect(0);
    }
}

com_id_list_t MultiComMgr::getReadyDevList()
{
    com_id_list_t idList;
    for (auto &id : m_readyIdSet) {
        idList.push_back(id);
    }
    return idList;
}

const com_dev_data_t &MultiComMgr::devData(com_id_t id, bool *valid /* = nullptr */)
{
    auto it = m_ptrMap.left.find(id);
    if (valid != nullptr) {
        *valid = (it != m_ptrMap.left.end());
    }
    if (it == m_ptrMap.left.end()) {
        return m_datMap.at(ComInvalidId);
    } else {
        return m_datMap.at(it->get_left());
    }
}

bool MultiComMgr::putCommand(com_id_t id, const ComCommandPtr &command)
{
    auto it = m_ptrMap.left.find(id);
    if (it == m_ptrMap.left.end()) {
        return false;
    }
    m_ptrMap.left.at(id)->putCommand(command);
    return true;
}

void MultiComMgr::initConnection(const com_ptr_t &comPtr)
{
    m_comPtrs.push_back(comPtr);
    m_ptrMap.insert(com_ptr_map_val_t(comPtr->id(), comPtr.get()));
    m_datMap.emplace(comPtr->id(), com_dev_data_t());
    m_serialNumberSet.insert(comPtr->serialNumber());

    comPtr->Bind(COM_CONNECTION_READY_EVENT, [this](const ComConnectionReadyEvent &event) {
        m_readyIdSet.insert(event.id);
        QueueEvent(event.Clone());
    });
    comPtr->Bind(COM_SEND_GCODE_PROGRESS_EVENT, [this](const ComSendGcodeProgressEvent &event) {
        QueueEvent(event.Clone());
    });
    comPtr->Bind(COM_CONNECTION_EXIT_EVENT, &MultiComMgr::onConnectionExit, this);
    comPtr->Bind(COM_DEV_DETAIL_UPDATE_EVENT, &MultiComMgr::onDevDetailUpdate, this);
    comPtr->connect();
}

void MultiComMgr::onWanDevUpdated(const WanDevUpdateEvent &event)
{
    if (m_wanDevUpdateThd->getAccessToken() != event.accessToken) {
        return;
    }
    fnet::FreeInDestructorArg freeDevInfos(event.devInfos, m_networkIntfc->freeWanDevList, event.devCnt);
    std::set<std::string> devIdSet;
    for (int i = 0; i < event.devCnt; ++i) {
        devIdSet.insert(event.devInfos[i].serialNumber);
    }
    for (auto &comPtr : m_comPtrs) {
        if (comPtr->connectMode() == COM_CONNECT_WAN
         && devIdSet.find(comPtr->serialNumber()) == devIdSet.end()) {
            comPtr.get()->disconnect(0);
        } else {
            comPtr->setAccessToken(event.accessToken);
        }
    }
    for (int i = 0; i < event.devCnt; ++i) {
        if (m_serialNumberSet.find(event.devInfos[i].serialNumber) == m_serialNumberSet.end()) {
            initConnection(com_ptr_t(new ComConnection(
                m_idNum++, event.accessToken, event.devInfos[i], m_networkIntfc.get())));
        }
    }
}

void MultiComMgr::onConnectionExit(const ComConnectionExitEvent &event)
{
    ComConnection *comConnection = m_ptrMap.left.at(event.id);
    comConnection->joinThread();
    m_networkIntfc->freeDevDetail(m_datMap.at(event.id).devDetail);
    m_readyIdSet.erase(event.id);
    m_serialNumberSet.erase(comConnection->serialNumber());
    m_datMap.erase(event.id);
    m_ptrMap.left.erase(event.id);
    m_comPtrs.remove_if([comConnection](auto &ptr) { return ptr.get() == comConnection; });
    QueueEvent(event.Clone());
}

void MultiComMgr::onDevDetailUpdate(const ComDevDetailUpdateEvent &event)
{
    fnet_dev_detail_t *&devDetail = m_datMap.at(event.id).devDetail;
    m_networkIntfc->freeDevDetail(devDetail);
    devDetail = event.devDetail;
    QueueEvent(event.Clone());
}

}} // namespace Slic3r::GUI
