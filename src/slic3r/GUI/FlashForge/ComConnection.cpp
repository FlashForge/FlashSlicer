#include "ComConnection.hpp"

namespace Slic3r { namespace GUI {

void ComConnection::connect()
{
    m_thread.reset(new boost::thread(boost::bind(&ComConnection::run, this)));
}

void ComConnection::disconnect()
{
    m_thread->join();
}

void ComConnection::run()
{
}

}} // namespace Slic3r::GUI
