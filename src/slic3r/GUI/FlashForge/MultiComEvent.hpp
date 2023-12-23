#ifndef slic3r_GUI_MultiComEvent_hpp_
#define slic3r_GUI_MultiComEvent_hpp_

#include <wx/event.h>
#include "MultiComDef.hpp"

namespace Slic3r { namespace GUI {

struct ComConnectionReadyEvent : public wxCommandEvent {
    ComConnectionReadyEvent(wxEventType type, com_id_t _id)
        : wxCommandEvent(type), id(_id) {
    }
    ComConnectionReadyEvent *Clone() const {
        return new ComConnectionReadyEvent(GetEventType(), id);
    }
    com_id_t id;
};

struct ComConnectionExitEvent : public wxCommandEvent {
    ComConnectionExitEvent(wxEventType type,  com_id_t _id, ComErrno _exitCode)
        : wxCommandEvent(type), id(_id), exitCode(_exitCode) {
    }
    ComConnectionExitEvent *Clone() const {
        return new ComConnectionExitEvent(GetEventType(), id, exitCode);
    }
    com_id_t id;
    ComErrno exitCode;
};

wxDECLARE_EVENT(COM_CONNECTION_READY_EVENT, ComConnectionReadyEvent);
wxDECLARE_EVENT(COM_CONNECTION_EXIT_EVENT, ComConnectionExitEvent);

}} // namespace Slic3r::GUI

#endif
