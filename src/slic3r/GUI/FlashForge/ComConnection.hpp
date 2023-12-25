#ifndef slic3r_GUI_ComConnection_hpp_
#define slic3r_GUI_ComConnection_hpp_

#include <memory>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <wx/event.h>
#include "ComCommandQue.hpp"
#include "FlashNetworkIntfc.h"
#include "MultiComDef.hpp"
#include "WaitEvent.hpp"

namespace Slic3r { namespace GUI {

class ComConnection : public wxEvtHandler
{
public:
    ComConnection(com_id_t id, const fnet_lan_dev_info_t &devInfo, fnet::FlashNetworkIntfc *networkIntfc);
    
    ComConnection(com_id_t id, const std::string &accessToken, const fnet_wan_dev_info_t &devInfo,
        fnet::FlashNetworkIntfc *networkIntfc);

    com_id_t id() const { return m_id; }

    ComConnectMode connectMode() const { return m_connectMode; }

    const std::string &serialNumber() const { return m_serialNumber; }

    void connect();

    void disconnect(unsigned int waitMilliseconds = -1);

    void joinThread();

    void setAccessToken(const std::string &accessToken);

private:
    void run();

    ComErrno commandLoop();

    std::string getAccessToken();

private:
    com_id_t                        m_id;
    ComConnectMode                  m_connectMode;
    std::string                     m_serialNumber;
    std::string                     m_accessToken;
    std::string                     m_deviceId;
    boost::mutex                    m_tokenMutex;
    WaitEvent                       m_exitEvent;
    ComCommandQue                   m_commandQue;
    fnet::FlashNetworkIntfc        *m_networkIntfc;
    std::unique_ptr<boost::thread>  m_thread;
};

}} // namespace Slic3r::GUI

#endif
