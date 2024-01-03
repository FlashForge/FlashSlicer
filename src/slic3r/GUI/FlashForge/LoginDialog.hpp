#ifndef slic3r_GUI_LoginDialog_hpp_
#define slic3r_GUI_LoginDialog_hpp_

#include <wx/wx.h>
#include <wx/intl.h>

#include "slic3r/GUI/GUI_Utils.hpp"
#include "slic3r/GUI/Widgets/Button.hpp"
//#include "slic3r/GUI/Widgets/TextInput.hpp"
//#include "slic3r/GUI/Widgets/StaticLine.hpp"

namespace Slic3r { 
namespace GUI {

//wxDEFINE_EVENT(EVT_SET_FINISH_MAPPING, wxCommandEvent);
wxDECLARE_EVENT(EVT_UPDATE_TEXT_LOGIN, wxCommandEvent);

class CountdownButton : public Button
{
public:
    CountdownButton(wxWindow* parent, wxString text, wxString icon = "", long style = 0, int iconSize = 0, wxWindowID btn_id = wxID_ANY);
    void startTimer(){
        // 启动定时器，每秒钟更新一次按钮上的文本
        m_timer.Start(1000);
    }

private:
    void OnTimer(wxTimerEvent& event);

    wxTimer m_timer;
    int m_countdown;
    wxWindow* m_parent;
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
    UsrnameTextCtrl(wxBitmap usrnamebitmap,wxWindow *parent, wxWindowID id = wxID_ANY);

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
    PasswordTextCtrl(wxBitmap lockbitmap,wxBitmap eyeoffbitmapBtn,wxBitmap eyeonbitmapBtn,wxWindow *parent, wxWindowID id = wxID_ANY);

    wxString GetValue(){
        return m_password_text_ctrl->GetValue();
    }

    void OnTextCtrlChanged(wxCommandEvent& event);

private:
    void OnShowPasswordButtonClicked(wxMouseEvent& event);

private:
    wxTextCtrl*      m_password_text_ctrl{nullptr};
    wxTextCtrl*      m_plain_text_ctrl{nullptr};
    wxBitmap         m_eye_off_bitmap;
    wxBitmap         m_eye_on_bitmap;
    wxStaticBitmap*  m_lock_staticbitmap{nullptr};
    wxStaticBitmap*  m_showPassword_staticbitmap{nullptr};
    bool             m_encrypt = true;
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