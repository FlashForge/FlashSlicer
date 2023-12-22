#ifndef slic3r_GUI_MultiComEvent_hpp_
#define slic3r_GUI_MultiComEvent_hpp_

#include <wx/event.h>

namespace Slic3r { namespace GUI {

wxDECLARE_EVENT(COM_CONNECTION_READY_EVENT, wxCommandEvent); // call getInt() to get the connection id

}} // namespace Slic3r::GUI

#endif
