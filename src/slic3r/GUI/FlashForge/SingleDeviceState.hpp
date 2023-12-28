#ifndef slic3r_GUI_SingleDeviceState_hpp_
#define slic3r_GUI_SingleDeviceState_hpp_

#include <wx/wx.h>
#include <wx/intl.h>
#include <wx/panel.h>

#include "slic3r/GUI/GUI_Utils.hpp"
#include "slic3r/GUI/Widgets/Label.hpp"
#include "slic3r/GUI/Widgets/Button.hpp"
#include "slic3r/GUI/wxMediaCtrl2.h"
#include "slic3r/GUI/MediaPlayCtrl.h"
#include "slic3r/GUI/Widgets/ProgressBar.hpp"
//#include "slic3r/GUI/Widgets/StaticLine.hpp"

namespace Slic3r { 
namespace GUI {

class MaterialPanel : public wxPanel
{
public:
    MaterialPanel(wxWindow* parent);
    ~MaterialPanel();
    void create_panel(wxWindow* parent);

private:
    wxPanel* m_panel_printing_title;
    wxStaticText*  m_staticText_printing;
    wxStaticText*   m_staticText_subtask_value;
};

class SingleDeviceState : public wxScrolledWindow
{
public:
    SingleDeviceState(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, 
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
    ~SingleDeviceState();

    wxBoxSizer* create_top_device_info_page();
    wxBoxSizer *create_monitoring_page();
    wxBoxSizer *create_machine_control_page(wxWindow *parent);

    void msw_rescale();

protected:
    wxPanel*  m_panel_monitoring_title;
    Label*   m_staticText_monitoring;
    wxMediaCtrl2*  m_media_ctrl;
    MediaPlayCtrl* m_media_play_ctrl;
    wxPanel*       m_machine_ctrl_panel;
    wxPanel*       m_panel_separator_right;
    wxPanel*       m_panel_separotor_bottom;
    MaterialPanel* m_material_panel;

    wxPanel*       m_panel_control_title;
    Label *        m_staticText_control;

    wxPanel* m_panel_top_title;
    Label*   m_staticText_device_name;
    Label*   m_staticText_device_position;
    Label*   m_staticText_device_tip;
    Label*   m_staticText_device_info;
    Button*  m_clear_button;

    wxBitmap    m_material_weight_pic;
    wxBitmap    m_material_pic;
    wxStaticBitmap*  m_material_weight_staticbitmap;
    wxStaticBitmap*  m_material_staticbitmap;

    wxPanel*       m_panel_control_info;
    wxPanel*       m_panel_separator_top_title_right;
    wxPanel*       m_panel_top_right_info;

    Label*   m_staticText_file_name;
    Label*   m_staticText_device_state;
    Label*   m_staticText_count_time;
    Label*   m_staticText_time_label;
    ProgressBar* m_progress_bar;

    Label* m_material_weight_label;
};



} // namespace GUI
} // namespace Slic3r

#endif