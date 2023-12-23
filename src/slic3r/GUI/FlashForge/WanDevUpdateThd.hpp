#ifndef slic3r_GUI_WanDevUpdateThd_hpp_
#define slic3r_GUI_WanDevUpdateThd_hpp_

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <wx/event.h>
#include "FlashNetworkIntfc.h"
#include "WaitEvent.hpp"

namespace Slic3r { namespace GUI {

struct WanDevUpdateEvent : public wxCommandEvent {
    WanDevUpdateEvent(wxEventType _type, const std::string &_accessToken,
        fnet_wan_dev_info_t *_devInfos, int _devCnt) {
        SetEventType(_type);
        accessToken = _accessToken;
        devInfos = _devInfos;
        devCnt = _devCnt;
    }
    std::string accessToken;
    fnet_wan_dev_info_t *devInfos;
    int devCnt;
};
wxDECLARE_EVENT(WAN_DEV_UPDATE_EVENT, WanDevUpdateEvent);

class WanDevUpdateThd : public wxEvtHandler
{
public:
    WanDevUpdateThd(fnet::FlashNetworkIntfc *networkIntfc);

    void exit();

    std::string getAccessToken();

    void setToken(const std::string &accessToken);

private:
    void run();

private:
    std::string              m_accessToken;
    boost::mutex             m_tokenMutex;
    WaitEvent                m_exitEvent;
    boost::thread            m_thread;
    fnet::FlashNetworkIntfc *m_networkIntfc;
};

}} // namespace Slic3r::GUI

#endif
