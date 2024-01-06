#ifndef slic3r_GUI_ReLoginDialog_hpp_
#define slic3r_GUI_ReLoginDialog_hpp_

#include <wx/wx.h>
#include <wx/intl.h>

#include "slic3r/GUI/GUI_Utils.hpp"
#include "slic3r/GUI/Widgets/Button.hpp"
#include "slic3r/GUI/Widgets/Label.hpp"

namespace Slic3r { 
namespace GUI {
class ReLoginDialog : public DPIDialog
{
public:
    ReLoginDialog();
    virtual ~ReLoginDialog();

    void onReloginBtnClicked(wxCommandEvent& event);
    void onLoginoutBtnClicked(wxCommandEvent& event);

protected:
    void on_dpi_changed(const wxRect &suggested_rect) override;

private:
    wxPanel* m_panel_page{nullptr};

    wxBitmap m_usr_pic;
    Label* m_usr_name{nullptr};

    wxStaticBitmap*  m_usr_pic_staticbitmap;

    wxButton* m_re_login_button {nullptr};
    wxButton* m_login_out_button {nullptr};

};
} // namespace GUI
} // namespace Slic3r

#endif