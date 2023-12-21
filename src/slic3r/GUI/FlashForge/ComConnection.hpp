#ifndef slic3r_GUI_ComConnection_hpp_
#define slic3r_GUI_ComConnection_hpp_

#include <memory>
#include <boost/thread/thread.hpp> 

namespace Slic3r { namespace GUI {

class ComConnection
{
public:
    void connect();

    void disconnect();

private:
    void run();

private:
    std::unique_ptr<boost::thread> m_thread;
};

}} // namespace Slic3r::GUI

#endif
