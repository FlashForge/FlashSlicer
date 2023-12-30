#ifndef slic3r_GUI_MultiComEvent_hpp_
#define slic3r_GUI_MultiComEvent_hpp_

#include <wx/event.h>
#include "MultiComDef.hpp"

namespace Slic3r { namespace GUI {

struct ComEvent : public wxCommandEvent
{
    ComEvent(wxEventType type, com_id_t _id, int _commandId)
        : wxCommandEvent(type)
        , id(_id)
        , commandId(_commandId)
    {
    }
    com_id_t id;
    int commandId;
};

struct ComConnectionReadyEvent : public ComEvent
{
    ComConnectionReadyEvent(wxEventType type, com_id_t _id)
        : ComEvent(type, _id, 0)
    {
    }
    ComConnectionReadyEvent *Clone() const
    {
        return new ComConnectionReadyEvent(GetEventType(), id);
    }
};

struct ComConnectionExitEvent : public ComEvent
{
    ComConnectionExitEvent(wxEventType type, com_id_t _id, ComErrno _ret)
        : ComEvent(type, _id, 0)
        , ret(_ret)
    {
    }
    ComConnectionExitEvent *Clone() const
    {
        return new ComConnectionExitEvent(GetEventType(), id, ret);
    }
    ComErrno ret;
};

struct ComDevDetailUpdateEvent : public ComEvent
{
    ComDevDetailUpdateEvent(wxEventType type,  com_id_t _id, int _commandId, fnet_dev_detail_t *_devDetail)
        : ComEvent(type, _id, _commandId)
        , devDetail(_devDetail)
    {
    }
    ComDevDetailUpdateEvent *Clone() const
    {
        return new ComDevDetailUpdateEvent(GetEventType(), id, commandId, devDetail);
    }
    fnet_dev_detail_t *devDetail;
};

struct ComSendGcodeProgressEvent : public ComEvent
{
    ComSendGcodeProgressEvent(wxEventType type, com_id_t _id, int _commandId, double _now, double _total) 
        : ComEvent(type, _id, _commandId)
        , now(_now)
        , total(_total)
    {
    }
    ComSendGcodeProgressEvent *Clone() const
    {
        return new ComSendGcodeProgressEvent(GetEventType(), id, commandId, now, total);
    }
    double now;
    double total;
};

struct ComSendGcodeFinishEvent : public ComEvent
{
    ComSendGcodeFinishEvent(wxEventType type, com_id_t _id, int _commandId, ComErrno _ret)
        : ComEvent(type, _id, _commandId)
        , ret(_ret)
    {
    }
    ComSendGcodeFinishEvent *Clone() const
    {
        return new ComSendGcodeFinishEvent(GetEventType(), id, commandId, ret);
    }
    ComErrno ret;
};

wxDECLARE_EVENT(COM_CONNECTION_READY_EVENT, ComConnectionReadyEvent);
wxDECLARE_EVENT(COM_CONNECTION_EXIT_EVENT, ComConnectionExitEvent);
wxDECLARE_EVENT(COM_DEV_DETAIL_UPDATE_EVENT, ComDevDetailUpdateEvent);
wxDECLARE_EVENT(COM_SEND_GCODE_PROGRESS_EVENT, ComSendGcodeProgressEvent);
wxDECLARE_EVENT(COM_SEND_GCODE_FINISH_EVENT, ComSendGcodeFinishEvent);

}} // namespace Slic3r::GUI

#endif
