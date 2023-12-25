#ifndef slic3r_GUI_LoginDialog_hpp_
#define slic3r_GUI_LoginDialog_hpp_

#include <wx/wx.h>
#include <wx/intl.h>

#include "slic3r/GUI/GUI_Utils.hpp"

namespace Slic3r { 
namespace GUI {

class CountdownButton : public wxButton
{
public:
    CountdownButton(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0)
        : wxButton(parent, id, label, pos, size, style), m_countdown(60)
    {
        // 创建一个 wxTimer 对象，并将其绑定到当前控件上
        m_timer.Bind(wxEVT_TIMER, &CountdownButton::OnTimer, this);

    }
    void startTimer(){
        // 启动定时器，每秒钟更新一次按钮上的文本
        m_timer.Start(1000);
    }

private:
    void OnTimer(wxTimerEvent& event)
    {
        // 更新倒计时
        m_countdown--;

        if (m_countdown > 0)
        {
            // 更新按钮上的文本
            SetLabel(wxString::Format("%d second", m_countdown));
        }
        else
        {
            // 停止定时器
            m_timer.Stop();

            // 恢复按钮上原来的文本
            SetLabel("Get Code");
        }
    }

    wxTimer m_timer;
    int m_countdown;
};



class LoginDialog : public DPIDialog
{
public:
    LoginDialog();

protected:
    void on_dpi_changed(const wxRect &suggested_rect) override;

private:
    void setupLayoutPage1(wxBoxSizer* page1Sizer,wxPanel* parent);
    void setupLayoutPage2(wxBoxSizer* page2Sizer,wxPanel* parent);

};
} // namespace GUI
} // namespace Slic3r

#endif