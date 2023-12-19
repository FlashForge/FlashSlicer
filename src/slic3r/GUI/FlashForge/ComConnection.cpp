#include "ComConnection.hpp"

namespace Slic3r { namespace GUI {

void ComConnection::connect()
{
    Run();
}

wxThread::ExitCode ComConnection::Entry()
{
    return 0;
}

}} // namespace Slic3r::GUI
