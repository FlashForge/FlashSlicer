#include "UserDataUpdateThd.hpp"
#include <boost/bind/bind.hpp>
#include "FreeInDestructor.h"
#include "MultiComEvent.hpp"
#include "MultiComUtils.hpp"

namespace Slic3r { namespace GUI {

wxDEFINE_EVENT(WAN_DEV_UPDATE_EVENT, WanDevUpdateEvent);

UserDataUpdateThd::UserDataUpdateThd(fnet::FlashNetworkIntfc *networkIntfc)
    : m_thread(boost::bind(&UserDataUpdateThd::run, this))
    , m_networkIntfc(networkIntfc)
{
}

void UserDataUpdateThd::exit()
{
    m_loopWaitEvent.set(true);
    m_exitThread = true;
    m_thread.join();
}

std::string UserDataUpdateThd::getAccessToken()
{
    boost::mutex::scoped_lock lock(m_tokenMutex);
    return m_accessToken;
}

void UserDataUpdateThd::setToken(const std::string &accessToken)
{
    boost::mutex::scoped_lock lock(m_tokenMutex);
    m_accessToken = accessToken;
    m_loopWaitEvent.set(true);
}

void UserDataUpdateThd::run()
{
    while (!m_exitThread) {
        m_loopWaitEvent.set(false);
        std::string accessToken = getAccessToken();
        if (!accessToken.empty()) {
            updateWanDev(accessToken);
            updateUserProfile(accessToken);
        }
        m_loopWaitEvent.waitTrue(5000);
    }
}

void UserDataUpdateThd::updateUserProfile(const std::string &accessToken)
{
    if (accessToken == m_oldAccessToken) {
        return;
    }
    fnet_user_profile_t *fnetProfile;
    int fnetRet = m_networkIntfc->getUserProfile(accessToken.c_str(), &fnetProfile);
    ComErrno ret = MultiComUtils::networkIntfcRet2ComErrno(fnetRet);
    if (ret != COM_OK) {
        QueueEvent(new ComGetUserProfileEvent(COM_GET_USER_PROFILE_EVENT, com_user_profile_t(), ret));
        return;
    }
    fnet::FreeInDestructor freeProfile(fnetProfile, m_networkIntfc->freeUserProfile);
    com_user_profile_t userProfile;
    userProfile.nickname = fnetProfile->nickname;
    userProfile.headImgUrl = fnetProfile->headImgUrl;
    QueueEvent(new ComGetUserProfileEvent(COM_GET_USER_PROFILE_EVENT, userProfile, ret));
    m_oldAccessToken = accessToken;
}

void UserDataUpdateThd::updateWanDev(const std::string &accessToken)
{
    int devCnt;
    fnet_wan_dev_info_t *devInfos;
    if (m_networkIntfc->getWanDevList(accessToken.c_str(), &devInfos, &devCnt) == 0) {
        QueueEvent(new WanDevUpdateEvent(WAN_DEV_UPDATE_EVENT, accessToken, devInfos, devCnt));
    }
}

}} // namespace Slic3r::GUI
