#ifndef slic3r_GUI_MultiComEvent_hpp_
#define slic3r_GUI_MultiComEvent_hpp_

#include <wx/event.h>
#include "MultiComDef.hpp"

namespace Slic3r { namespace GUI {

struct ComConnectionReadyEvent : public wxCommandEvent
{
    ComConnectionReadyEvent(wxEventType type, com_id_t _id)
        : wxCommandEvent(type), id(_id)
    {
    }
    ComConnectionReadyEvent *Clone() const
    {
        return new ComConnectionReadyEvent(GetEventType(), id);
    }
    com_id_t id;
};

struct ComConnectionExitEvent : public wxCommandEvent
{
    ComConnectionExitEvent(wxEventType type,  com_id_t _id, ComErrno _exitCode)
        : wxCommandEvent(type), id(_id), exitCode(_exitCode)
    {
    }
    ComConnectionExitEvent *Clone() const
    {
        return new ComConnectionExitEvent(GetEventType(), id, exitCode);
    }
    com_id_t id;
    ComErrno exitCode;
};

struct ComDevDetailUpdateEvent : public wxCommandEvent
{
    ComDevDetailUpdateEvent(wxEventType type,  com_id_t _id, int _commandId,
        fnet_dev_detail_t *_devDetail)
        : wxCommandEvent(type), id(_id), commandId(_commandId), devDetail(_devDetail)
    {
    }
    ComDevDetailUpdateEvent *Clone() const
    {
        return new ComDevDetailUpdateEvent(GetEventType(), id, commandId, devDetail);
    }
    com_id_t id;
    int commandId;
    fnet_dev_detail_t *devDetail;
};

struct ComSendGcodeProgressEvent : public wxCommandEvent
{
    ComSendGcodeProgressEvent(wxEventType type, com_id_t _id, int _commandId,
        double _now, double _total) 
        : wxCommandEvent(type), id(_id), commandId(_commandId), now(_now), total(_total)
    {
    }
    ComSendGcodeProgressEvent *Clone() const
    {
        return new ComSendGcodeProgressEvent(GetEventType(), id, commandId, now, total);
    }
    com_id_t id;
    int commandId;
    double now;
    double total;
};

wxDECLARE_EVENT(COM_CONNECTION_READY_EVENT, ComConnectionReadyEvent);
wxDECLARE_EVENT(COM_CONNECTION_EXIT_EVENT, ComConnectionExitEvent);
wxDECLARE_EVENT(COM_DEV_DETAIL_UPDATE_EVENT, ComDevDetailUpdateEvent);
wxDECLARE_EVENT(COM_SEND_GCODE_PROGRESS_EVENT, ComSendGcodeProgressEvent);

}} // namespace Slic3r::GUI

#endif
