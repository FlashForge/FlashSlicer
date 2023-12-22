#ifndef slic3r_GUI_ComConnection_hpp_
#define slic3r_GUI_ComConnection_hpp_

#include <memory>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "FlashNetworkIntfc.h"
#include "MultiComDef.hpp"
#include "WaitEvent.hpp"

namespace Slic3r { namespace GUI {

class ComConnection
{
public:
    ComConnection(const fnet_lan_dev_info_t &devInfo, fnet::FlashNetworkIntfc *networkIntfc);
    
    ComConnection(const std::string &accessToken, const fnet_wan_dev_info_t &devInfo,
        fnet::FlashNetworkIntfc *networkIntfc);

    ComConnectMode connectMode() const { return m_connectMode; }

    const std::string &devId() const { return m_devId; }

    bool isAlive() { return !m_exitEvent.get(); }

    void connect();

    void disconnect(unsigned int waitMilliseconds = -1);

    void joinThread();

    void setAccessToken(const std::string &accessToken);

private:
    void run();

    std::string getAccessToken();

private:
    ComConnectMode                  m_connectMode;
    std::string                     m_accessToken;
    std::string                     m_devId;
    boost::mutex                    m_tokenMutex;
    WaitEvent                       m_exitEvent;
    fnet::FlashNetworkIntfc        *m_networkIntfc;
    std::unique_ptr<boost::thread>  m_thread;
};

}} // namespace Slic3r::GUI

#endif
