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
    return m_networkIntfc->isOk();
}

void MultiComMgr::uninitalize()
{
    m_networkIntfc.reset(nullptr);
}

ComErrno MultiComMgr::getLanDevList(std::vector<fnet_lan_dev_info> &devInfos)
{
    if (m_networkIntfc.get() == nullptr) {
        return COM_UNINITIALIZED;
    }
    int devCnt;
    fnet_lan_dev_info *fnetDevInfos;
    if (m_networkIntfc->getLanDevList(&fnetDevInfos, &devCnt) != 0) {
        return COM_ERROR;
    }
    fnet::FreeInDestructor freeDevInfos(fnetDevInfos, m_networkIntfc->freeLanDevInfos);
    devInfos.clear();
    for (int i = 0; i < devCnt; ++i) {
        devInfos.push_back(fnetDevInfos[i]);
    }
    return COM_OK;
}

ComErrno MultiComMgr::addLanDev(const fnet_lan_dev_info &devInfo)
{
    initConnection(com_ptr_t(new ComConnection));
    return COM_OK;
}

ComErrno MultiComMgr::getTokenByPassword(const std::string &userName, const std::string &password,
    com_token_info_t &tokenInfo)
{
    if (m_networkIntfc.get() == nullptr) {
        return COM_UNINITIALIZED;
    }
    fnet_token_info_t *fnetTokenInfo;
    if (m_networkIntfc->getTokenByPassword(userName.c_str(), password.c_str(), &fnetTokenInfo) != 0) {
        return COM_ERROR;
    }
    fnet::FreeInDestructor freeTokenInfo(fnetTokenInfo, m_networkIntfc->freeTokenInfo);
    tokenInfo.expiresIn = fnetTokenInfo->expiresIn;
    tokenInfo.accessToken = fnetTokenInfo->accessToken;
    tokenInfo.refreshToken = fnetTokenInfo->refreshToken;
    return COM_OK;
}

ComErrno MultiComMgr::refreshToken(const std::string &refreshToken, com_token_info_t &tokenInfo)
{
    if (m_networkIntfc.get() == nullptr) {
        return COM_UNINITIALIZED;
    }
    fnet_token_info_t *fnetTokenInfo;
    if (m_networkIntfc->refreshToken(refreshToken.c_str(), &fnetTokenInfo) != 0) {
        return COM_ERROR;
    }
    fnet::FreeInDestructor freeTokenInfo(fnetTokenInfo, m_networkIntfc->freeTokenInfo);
    tokenInfo.expiresIn = fnetTokenInfo->expiresIn;
    tokenInfo.accessToken = fnetTokenInfo->accessToken;
    tokenInfo.refreshToken = fnetTokenInfo->refreshToken;
    return COM_OK;
}

ComErrno MultiComMgr::addWanDevList(const std::string &accessToken)
{
    if (m_networkIntfc.get() == nullptr) {
        return COM_UNINITIALIZED;
    }
    int devCnt;
    fnet_wan_dev_info_t *fnetWanDevInfos;
    if (m_networkIntfc->getWanDevList(accessToken.c_str(), &fnetWanDevInfos, &devCnt) != 0) {
        return COM_ERROR;
    }
    for (int i = 0; i < devCnt; ++i) {
        initConnection(com_ptr_t(new ComConnection));
    }
    return COM_OK;
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
    comConnection->Wait();
    m_datMap.erase(m_ptrMap.right.find(comConnection)->get_left());
    m_ptrMap.right.erase(comConnection);
    m_comPtrs.remove_if([comConnection](const com_ptr_t &ptr) {
        return ptr.get() == comConnection;
   });
}

}} // namespace Slic3r::GUI
