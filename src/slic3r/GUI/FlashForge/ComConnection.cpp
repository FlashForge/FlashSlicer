#include "ComConnection.hpp"

namespace Slic3r { namespace GUI {

wxDEFINE_EVENT(COM_CONNECTION_EXIT_EVENT, wxCommandEvent);

ComConnection::ComConnection(const fnet_lan_dev_info_t &devInfo, fnet::FlashNetworkIntfc *networkIntfc)
    : m_connectMode(COM_CONNECT_LAN)
    , m_devId(devInfo.id)
    , m_networkIntfc(networkIntfc)
{
}

ComConnection::ComConnection(const std::string &accessToken, const fnet_wan_dev_info_t &devInfo,
    fnet::FlashNetworkIntfc *networkIntfc)
    : m_connectMode(COM_CONNECT_WAN)
    , m_accessToken(accessToken)
    , m_devId(devInfo.id)
    , m_networkIntfc(networkIntfc)
{
}

void ComConnection::connect()
{
    m_thread.reset(new boost::thread(boost::bind(&ComConnection::run, this)));
}

void ComConnection::disconnect(unsigned int waitMilliseconds/*= -1*/)
{
    m_exitEvent.set(true);
    if (waitMilliseconds > 0) {
        m_thread->try_join_for(boost::chrono::milliseconds(waitMilliseconds));
    }
}

void ComConnection::joinThread()
{
    m_thread->join();
}

void ComConnection::setAccessToken(const std::string &accessToken)
{
    boost::mutex::scoped_lock lock(m_tokenMutex);
    m_accessToken = accessToken;
}

void ComConnection::run()
{
    while (!m_exitEvent.get()) {
        m_exitEvent.waitTrue(5000);
    }
    QueueEvent(wxCommandEvent(COM_CONNECTION_EXIT_EVENT).Clone());
}

std::string ComConnection::getAccessToken()
{
    boost::mutex::scoped_lock lock(m_tokenMutex);
    return m_accessToken;
}

}} // namespace Slic3r::GUI
