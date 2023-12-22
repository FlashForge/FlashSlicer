#include "WanDevUpdateThd.hpp"
#include <boost/bind/bind.hpp>

namespace Slic3r { namespace GUI {

wxDEFINE_EVENT(WAN_DEV_UPDATE_EVENT, WanDevUpdateEvent);

WanDevUpdateThd::WanDevUpdateThd(fnet::FlashNetworkIntfc *networkIntfc)
    : m_thread(boost::bind(&WanDevUpdateThd::run, this))
    , m_networkIntfc(networkIntfc)
{
}

void WanDevUpdateThd::exit()
{
    m_exitEvent.set(true);
    m_thread.join();
}

std::string WanDevUpdateThd::getAccessToken()
{
    boost::mutex::scoped_lock lock(m_tokenMutex);
    return m_accessToken;
}

void WanDevUpdateThd::setToken(const std::string &accessToken)
{
    boost::mutex::scoped_lock lock(m_tokenMutex);
    m_accessToken = accessToken;
}

void WanDevUpdateThd::run()
{
    while (!m_exitEvent.get()) {
        std::string accessToken = getAccessToken();
        if (!accessToken.empty()) {
            int devCnt;
            fnet_wan_dev_info_t *devInfos;
            if (m_networkIntfc->getWanDevList(accessToken.c_str(), &devInfos, &devCnt) == 0) {
                WanDevUpdateEvent event;
                event.SetEventType(WAN_DEV_UPDATE_EVENT);
                event.accessToken = accessToken;
                event.devInfos = devInfos;
                event.devCnt = devCnt;
                QueueEvent(event.Clone());
            }
        }
        m_exitEvent.waitTrue(5000);
    }
}

}} // namespace Slic3r::GUI
