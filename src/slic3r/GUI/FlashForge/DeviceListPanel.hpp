#ifndef slic3r_DeviceListPanel_hpp_
#define slic3r_DeviceListPanel_hpp_

#include <wx/panel.h>
#include "slic3r/GUI/Widgets/Button.hpp"

namespace Slic3r {
namespace GUI {
class DeviceListPanel : public wxPanel
{
protected:
    Button* m_button_add_machine;
	wxStaticText* m_staticText_add_machine;
	wxStaticBitmap* m_bitmap_empty;

public:
    DeviceListPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, 
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
    ~DeviceListPanel();

    void msw_rescale();
};

} // GUI
} // Slic3r

#endif /* slic3r_DeviceListPanel_hpp_ */
