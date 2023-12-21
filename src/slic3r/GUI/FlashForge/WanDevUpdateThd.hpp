#ifndef slic3r_GUI_WanDevUpdateThd_hpp_
#define slic3r_GUI_WanDevUpdateThd_hpp_

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "FlashNetworkIntfc.h"
#include "WaitEvent.hpp"

namespace Slic3r { namespace GUI {

class WanDevUpdateThd
{
public:
    WanDevUpdateThd(fnet::FlashNetworkIntfc *networkIntfc);

    void exit();

    void setToken(const std::string &accessToken);

private:
    void run();

    std::string getAccessToken();

private:
    std::string              m_accessToken;
    boost::mutex             m_tokenMutex;
    WaitEvent                m_exitEvent;
    boost::thread            m_thread;
    fnet::FlashNetworkIntfc *m_networkIntfc;
};

}} // namespace Slic3r::GUI

#endif
