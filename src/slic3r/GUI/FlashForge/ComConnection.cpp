#include "ComConnection.hpp"
#include "MultiComEvent.hpp"

namespace Slic3r { namespace GUI {

ComConnection::ComConnection(com_id_t id, const fnet_lan_dev_info_t &devInfo,
    fnet::FlashNetworkIntfc *networkIntfc)
    : m_id(id)
    , m_connectMode(COM_CONNECT_LAN)
    , m_serialNumber(devInfo.serialNumber)
    , m_networkIntfc(networkIntfc)
{
}

ComConnection::ComConnection(com_id_t id, const std::string &accessToken,
    const fnet_wan_dev_info_t &devInfo, fnet::FlashNetworkIntfc *networkIntfc)
    : m_id(id)
    , m_connectMode(COM_CONNECT_WAN)
    , m_serialNumber(devInfo.serialNumber)
    , m_accessToken(accessToken)
    , m_deviceId(devInfo.devId)
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
    QueueEvent(new ComConnectionReadyEvent(COM_CONNECTION_READY_EVENT, m_id));
    ComErrno exitCode = commandLoop();
    QueueEvent(new ComConnectionExitEvent(COM_CONNECTION_EXIT_EVENT, m_id, exitCode));
}

ComErrno ComConnection::commandLoop()
{
    int errorCnt = 0;
    while (!m_exitEvent.get()) {
        ComCommandPtr frontCommand = m_commandQue.popFront(100);
        if (frontCommand.get() != nullptr) {
            ComErrno ret = frontCommand->exec(m_networkIntfc);
            if (ret == COM_OK) {
                errorCnt = 0;
            } else if (++errorCnt > 5) {
                return ret;
            }
        }
    }
    return COM_OK;
}

std::string ComConnection::getAccessToken()
{
    boost::mutex::scoped_lock lock(m_tokenMutex);
    return m_accessToken;
}

}} // namespace Slic3r::GUI
