#include "WanDevUpdateThd.hpp"
#include <boost/bind/bind.hpp>
#include "FreeInDestructor.h"

namespace Slic3r { namespace GUI {

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
                fnet::FreeInDestructorArg freeDevInfos(devInfos, m_networkIntfc->freeWanDevList, devCnt);
            }
        }
        m_exitEvent.waitTrue(5000);
    }
}

std::string WanDevUpdateThd::getAccessToken()
{
    boost::mutex::scoped_lock lock(m_tokenMutex);
    return m_accessToken;
}

}} // namespace Slic3r::GUI
