#include "MultiComEvent.hpp"

namespace Slic3r { namespace GUI {

wxDEFINE_EVENT(COM_CONNECTION_READY_EVENT, ComConnectionReadyEvent);
wxDEFINE_EVENT(COM_CONNECTION_EXIT_EVENT, ComConnectionExitEvent);
wxDEFINE_EVENT(COM_DEV_DETAIL_UPDATE_EVENT, ComDevDetailUpdateEvent);

}} // namespace Slic3r::GUI
