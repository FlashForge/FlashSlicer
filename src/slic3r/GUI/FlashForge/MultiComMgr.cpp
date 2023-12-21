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
    m_networkIntfc.reset(new fnet::FlashNetworkIntfc(newtworkDllPath.c_str()));
    if (!m_networkIntfc->isOk()) {
        return false;
    }
    m_wanDevUpdateThd.reset(new WanDevUpdateThd(m_networkIntfc.get()));
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
    initConnection(com_ptr_t(new ComConnection));
}

void MultiComMgr::setWanDevToken(const std::string &accessToken)
{
    m_wanDevUpdateThd->setToken(accessToken);
}

com_id_list_t MultiComMgr::getReadyDevList()
{
    com_id_list_t idList;
    for (auto &id : m_readyIdSet) {
        idList.push_back(id);
    }
    return idList;
}

const com_dev_data_t &MultiComMgr::devData(com_id_t &id, bool *valid /* = nullptr */)
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

void MultiComMgr::initConnection(const com_ptr_t &comPtr)
{
    int id = m_idNum++;
    m_comPtrs.push_back(comPtr);
    m_ptrMap.insert(com_ptr_map_val_t(id, comPtr.get()));
    m_datMap.emplace(id, com_dev_data_t());
    comPtr->connect();
}

void MultiComMgr::uninitConnection(ComConnection *comConnection)
{
    comConnection->disconnect();
    m_datMap.erase(m_ptrMap.right.find(comConnection)->get_left());
    m_ptrMap.right.erase(comConnection);
    m_comPtrs.remove_if([comConnection](const com_ptr_t &ptr) {
        return ptr.get() == comConnection;
   });
}

}} // namespace Slic3r::GUI
