#ifndef slic3r_GUI_LoginDialog_hpp_
#define slic3r_GUI_LoginDialog_hpp_

#include <wx/wx.h>
#include <wx/intl.h>

#include "slic3r/GUI/GUI_Utils.hpp"

namespace Slic3r { 
namespace GUI {
class LoginDialog : public DPIDialog
{
public:
    LoginDialog();

protected:
    void on_dpi_changed(const wxRect &suggested_rect) override;

};
} // namespace GUI
} // namespace Slic3r

#endif