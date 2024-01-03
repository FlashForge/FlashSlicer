#include "MultiComEvent.hpp"

namespace Slic3r { namespace GUI {

wxDEFINE_EVENT(COM_CONNECTION_READY_EVENT, ComConnectionReadyEvent);
wxDEFINE_EVENT(COM_CONNECTION_EXIT_EVENT, ComConnectionExitEvent);
wxDEFINE_EVENT(COM_DEV_DETAIL_UPDATE_EVENT, ComDevDetailUpdateEvent);
wxDEFINE_EVENT(COM_SEND_GCODE_PROGRESS_EVENT, ComSendGcodeProgressEvent);
wxDEFINE_EVENT(COM_SEND_GCODE_FINISH_EVENT, ComSendGcodeFinishEvent);
wxDEFINE_EVENT(COM_GET_USER_PROFILE_EVENT, ComGetUserProfileEvent);

}} // namespace Slic3r::GUI
