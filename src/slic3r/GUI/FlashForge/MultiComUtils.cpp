#include "MultiComUtils.hpp"
#include "FreeInDestructor.h"
#include "MultiComMgr.hpp"

namespace Slic3r { namespace GUI {

ComErrno MultiComUtils::getLanDevList(std::vector<fnet_lan_dev_info> &devInfos)
{
    fnet::FlashNetworkIntfc *intfc = MultiComMgr::inst()->networkIntfc();
    if (intfc == nullptr) {
        return COM_UNINITIALIZED;
    }
    int devCnt;
    fnet_lan_dev_info *fnetDevInfos;
    if (intfc->getLanDevList(&fnetDevInfos, &devCnt) != 0) {
        return COM_ERROR;
    }
    fnet::FreeInDestructor freeDevInfos(fnetDevInfos, intfc->freeLanDevInfos);
    devInfos.clear();
    for (int i = 0; i < devCnt; ++i) {
        devInfos.push_back(fnetDevInfos[i]);
    }
    return COM_OK;
}

ComErrno MultiComUtils::getTokenByPassword(const std::string &userName, const std::string &password,
    com_token_info_t &tokenInfo)
{
    fnet::FlashNetworkIntfc *intfc = MultiComMgr::inst()->networkIntfc();
    if (intfc == nullptr) {
        return COM_UNINITIALIZED;
    }
    fnet_token_info_t *fnetTokenInfo;
    if (intfc->getTokenByPassword(userName.c_str(), password.c_str(), &fnetTokenInfo) != 0) {
        return COM_ERROR;
    }
    fnet::FreeInDestructor freeTokenInfo(fnetTokenInfo, intfc->freeTokenInfo);
    tokenInfo.expiresIn = fnetTokenInfo->expiresIn;
    tokenInfo.accessToken = fnetTokenInfo->accessToken;
    tokenInfo.refreshToken = fnetTokenInfo->refreshToken;
    return COM_OK;
}

ComErrno MultiComUtils::refreshToken(const std::string &refreshToken, com_token_info_t &tokenInfo)
{
    fnet::FlashNetworkIntfc *intfc = MultiComMgr::inst()->networkIntfc();
    if (intfc == nullptr) {
        return COM_UNINITIALIZED;
    }
    fnet_token_info_t *fnetTokenInfo;
    if (intfc->refreshToken(refreshToken.c_str(), &fnetTokenInfo) != 0) {
        return COM_ERROR;
    }
    fnet::FreeInDestructor freeTokenInfo(fnetTokenInfo, intfc->freeTokenInfo);
    tokenInfo.expiresIn = fnetTokenInfo->expiresIn;
    tokenInfo.accessToken = fnetTokenInfo->accessToken;
    tokenInfo.refreshToken = fnetTokenInfo->refreshToken;
    return COM_OK;
}

}} // namespace Slic3r::GUI
