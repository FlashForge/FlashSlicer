#ifndef slic3r_GUI_ComConnection_hpp_
#define slic3r_GUI_ComConnection_hpp_

#include <wx/thread.h>

namespace Slic3r { namespace GUI {

class ComConnection : public wxThread
{
public:
    void connect();

private:
    ExitCode Entry();
};

}} // namespace Slic3r::GUI

#endif
