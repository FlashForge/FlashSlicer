#ifndef slic3r_GUI_LoginDialog_hpp_
#define slic3r_GUI_LoginDialog_hpp_

#include <wx/wx.h>
#include <wx/intl.h>

#include "slic3r/GUI/GUI_Utils.hpp"
//#include "slic3r/GUI/Widgets/StaticLine.hpp"

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

class VerifycodeTextCtrl : public wxPanel
{
public:
    VerifycodeTextCtrl(wxBitmap verifycodebitmap,wxWindow *parent, wxWindowID id = wxID_ANY)
        : wxPanel(parent, id)
    {
        // 创建验证码输入框和锁图标
        m_verify_staticbitmap = new wxStaticBitmap(this, wxID_ANY,verifycodebitmap);
        m_verify_code_text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT | wxBORDER_NONE);
        m_verify_code_text_ctrl->SetHint("Code");
        m_verify_code_text_ctrl->SetMinSize(wxSize(171,33));

        // 创建垂直布局并添加控件
        wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
        hbox->Add(m_verify_staticbitmap,wxSizerFlags().Expand().Border(wxLEFT | wxRIGHT, 5));
        hbox->Add(m_verify_code_text_ctrl, wxSizerFlags().Expand().Border(wxTOP, 12));

        // 设置面板的布局
        SetSizerAndFit(hbox);
    }

    wxString GetValue(){
        return m_verify_code_text_ctrl->GetValue();
    }

private:
    wxTextCtrl*      m_verify_code_text_ctrl;
    wxStaticBitmap*  m_verify_staticbitmap;
};

class UsrnameTextCtrl : public wxPanel
{
public:
    UsrnameTextCtrl(wxBitmap usrnamebitmap,wxWindow *parent, wxWindowID id = wxID_ANY)
        : wxPanel(parent, id)
    {
        // 创建账号输入框和人像图标
        m_usr_staticbitmap = new wxStaticBitmap(this, wxID_ANY,usrnamebitmap);
        m_user_name_text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT | wxBORDER_NONE);
        m_user_name_text_ctrl->SetHint("Phone Number / email");
        m_user_name_text_ctrl->SetMinSize(wxSize(295,33));

        // 创建垂直布局并添加控件
        wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
        hbox->Add(m_usr_staticbitmap,wxSizerFlags().Expand().Border(wxLEFT | wxRIGHT, 5));
        hbox->Add(m_user_name_text_ctrl, wxSizerFlags().Expand().Border(wxTOP, 12));

        // 设置面板的布局
        SetSizerAndFit(hbox);
    }

    wxString GetValue(){
        return m_user_name_text_ctrl->GetValue();
    }

private:
    wxTextCtrl*      m_user_name_text_ctrl;
    wxStaticBitmap*  m_usr_staticbitmap;
};

class PasswordTextCtrl : public wxPanel
{
public:
    PasswordTextCtrl(wxBitmap lockbitmap,wxBitmap eyeoffbitmapBtn,wxBitmap eyeonbitmapBtn,wxWindow *parent, wxWindowID id = wxID_ANY)
        : wxPanel(parent, id)
    {
        m_eye_off_bitmap = eyeoffbitmapBtn;
        m_eye_on_bitmap = eyeonbitmapBtn;

        // 创建密码输入框和眼睛图标按钮
        m_lock_staticbitmap = new wxStaticBitmap(this, wxID_ANY,lockbitmap);
        m_password_text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD | wxTE_LEFT | wxBORDER_NONE);
        m_password_text_ctrl->SetHint("Password");
        m_password_text_ctrl->SetMinSize(wxSize(241,33));
        m_showPassword_staticbitmap = new wxStaticBitmap(this, wxID_ANY,eyeoffbitmapBtn);

        // 设置眼睛图标按钮的大小和工具提示文本
        m_showPassword_staticbitmap->SetSize(wxSize(20, 16));
        m_showPassword_staticbitmap->SetToolTip(wxT("Show password"));

        // 创建垂直布局并添加控件
        wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
        hbox->Add(m_lock_staticbitmap,wxSizerFlags().Expand().Border(wxLEFT | wxRIGHT, 5));
        hbox->Add(m_password_text_ctrl, wxSizerFlags().Expand().Border(wxTOP, 12));
        hbox->Add(m_showPassword_staticbitmap, wxSizerFlags().Center().Border(wxLEFT | wxRIGHT, 5));

        // 设置面板的布局
        SetSizerAndFit(hbox);

        // 连接信号和槽函数
        m_showPassword_staticbitmap->Bind(wxEVT_LEFT_UP, [this](wxMouseEvent& e){OnShowPasswordButtonClicked(e);});
    }

    wxString GetValue(){
        return m_password_text_ctrl->GetValue();
    }

private:
    void OnShowPasswordButtonClicked(wxMouseEvent& event)
    {
        if (m_password_text_ctrl->GetWindowStyleFlag() & wxTE_PASSWORD) {
            // 如果密码输入框当前为密码模式，则切换为明文模式
            m_password_text_ctrl->SetWindowStyleFlag(wxTE_PROCESS_ENTER);
            m_password_text_ctrl->SetWindowStyle(wxTE_PROCESS_ENTER);
            m_password_text_ctrl->SetValue(m_password_text_ctrl->GetValue());
            m_showPassword_staticbitmap->SetBitmap(m_eye_on_bitmap);
            m_showPassword_staticbitmap->SetToolTip(wxT("Hide password"));
            m_password_text_ctrl->Refresh();
        } else {
            // 如果密码输入框当前为明文模式，则切换为密码模式
            m_password_text_ctrl->SetWindowStyleFlag(wxTE_PASSWORD);
            m_password_text_ctrl->SetWindowStyle(wxTE_PASSWORD);
            m_password_text_ctrl->SetValue(m_password_text_ctrl->GetValue());
            m_showPassword_staticbitmap->SetBitmap(m_eye_off_bitmap);
            m_showPassword_staticbitmap->SetToolTip(wxT("Show password"));
            m_password_text_ctrl->Refresh();
        }
    }

private:
    wxTextCtrl*      m_password_text_ctrl;
    wxBitmap         m_eye_off_bitmap;
    wxBitmap         m_eye_on_bitmap;
    wxStaticBitmap*  m_lock_staticbitmap;
    wxStaticBitmap*  m_showPassword_staticbitmap;
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

    void onUsrNameOrPasswordChangedPage1(wxCommandEvent& event);
    void onAgreeCheckBoxChangedPage1(wxCommandEvent& event);

    void onUsrNameOrPasswordChangedPage2(wxCommandEvent& event);
    void onAgreeCheckBoxChangedPage2(wxCommandEvent& event);

private:
    wxStaticText* m_error_label {nullptr};
    wxButton* m_login_button_page1 {nullptr};
    UsrnameTextCtrl* m_usrname_page1 {nullptr};
    VerifycodeTextCtrl* m_verify_code {nullptr};
    wxCheckBox* m_login_check_box_page1 {nullptr};
    CountdownButton* m_get_code_button {nullptr};

    UsrnameTextCtrl* m_usrname_page2 {nullptr};
    PasswordTextCtrl* m_password {nullptr};
    wxButton* m_login_button_page2 {nullptr};
    wxCheckBox* m_login_check_box_page2 {nullptr};

    wxStaticText* m_protocol_page2{nullptr};
    wxStaticText* m_service_link_page2{nullptr};
    wxStaticText* m_privacy_policy_link_page2{nullptr};

    wxStaticLine* m_staticLine_verify{nullptr};
    wxStaticLine* m_staticLine_password{nullptr};

    //StaticLine* m_staticLine_verify{nullptr};
    //StaticLine* m_staticLine_password{nullptr};

};
} // namespace GUI
} // namespace Slic3r

#endif