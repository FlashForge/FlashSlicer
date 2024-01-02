#include "LoginDialog.hpp"

#include "slic3r/GUI/I18N.hpp"

#include "libslic3r/Utils.hpp"
#include "slic3r/GUI/GUI.hpp"
#include "slic3r/GUI/GUI_App.hpp"
#include "slic3r/GUI/MainFrame.hpp"
#include "slic3r/GUI/format.hpp"
#include "slic3r/GUI/Widgets/Button.hpp"

#include <wx/clipbrd.h>

namespace Slic3r {
namespace GUI {

    wxDEFINE_EVENT(EVT_UPDATE_TEXT_LOGIN, wxCommandEvent);

    static bool verify_btn_clicked = false;

    CountdownButton::CountdownButton(wxWindow* parent, wxString text, wxString icon /*= ""*/, long style /*= 0*/, int iconSize /*= 0*/, wxWindowID btn_id /*= wxID_ANY*/)
        : Button(parent,text,icon,style,iconSize,btn_id)
        , m_countdown(60)
        , m_parent(parent)
    {
        // 创建一个 wxTimer 对象，并将其绑定到当前控件上
        m_timer.Bind(wxEVT_TIMER, &CountdownButton::OnTimer, this);
        //Bind(EVT_UPDATE_TEXT_LOGIN, &CountdownButton::OnUpdateText, this);
    }

    void CountdownButton::OnTimer(wxTimerEvent& event)
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
            m_countdown = 60;

            // 恢复按钮上原来的文本
            SetLabel("Get Code");
        }
    }

    void CountdownButton::OnButtonClick(wxCommandEvent& event)
    {
        std::thread t([this]()
        {
            wxMutexLocker locker(m_mutex);
            while (true)
            {
                if(m_countdown <= 0){
                    m_countdown = 60;
                    break;
                }
                m_countdown --;
                //Sleep(1000);
                 wxThread::Sleep(1000);
                 {
                    wxCommandEvent* updateEvent = new wxCommandEvent(EVT_UPDATE_TEXT_LOGIN, wxID_ANY);
                    updateEvent->SetString(wxString::Format("%d second", m_countdown));
                    wxPostEvent(this->GetParent()->GetParent(), *updateEvent);
                 }
            }
        });
        t.detach();
    }

    // void CountdownButton::OnUpdateText(wxCommandEvent& event)
    // {
    //     this->SetLabel(event.GetString());
    //     event.Skip();
    // }

    UsrnameTextCtrl::UsrnameTextCtrl(wxBitmap usrnamebitmap,wxWindow *parent, wxWindowID id /*= wxID_ANY*/)
                    : wxPanel(parent, id)
    {
        // 创建账号输入框和人像图标
        m_usr_staticbitmap = new wxStaticBitmap(this, wxID_ANY,usrnamebitmap);
        m_user_name_text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT | wxBORDER_NONE);
        m_user_name_text_ctrl->SetHint("Phone Number / email");
        m_user_name_text_ctrl->SetMinSize(wxSize(295,33));
        wxTextValidator validator(wxFILTER_INCLUDE_CHAR_LIST);
        validator.SetCharIncludes("^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4})$|^1[3456789]\\d{9}$|[A-Za-z0-9@._-]");
        m_user_name_text_ctrl->SetValidator(validator);

        // 创建垂直布局并添加控件
        wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
        hbox->Add(m_usr_staticbitmap,wxSizerFlags().Expand().Border(wxLEFT | wxRIGHT, 5));
        hbox->Add(m_user_name_text_ctrl, wxSizerFlags().Expand().Border(wxTOP, 12));

        // 设置面板的布局
        SetSizerAndFit(hbox);
    }

    PasswordTextCtrl::PasswordTextCtrl(wxBitmap lockbitmap,wxBitmap eyeoffbitmapBtn,wxBitmap eyeonbitmapBtn,wxWindow *parent, wxWindowID id /*= wxID_ANY*/)
                    : wxPanel(parent, id)
    {
        m_eye_off_bitmap = eyeoffbitmapBtn;
        m_eye_on_bitmap = eyeonbitmapBtn;

        // 创建密码输入框和眼睛图标按钮
        m_lock_staticbitmap = new wxStaticBitmap(this, wxID_ANY,lockbitmap);
        m_password_text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT | wxBORDER_NONE);
        m_password_text_ctrl->SetWindowStyle(m_password_text_ctrl->GetWindowStyle() & wxTE_PASSWORD);
        m_password_text_ctrl->SetHint("Password");
        m_password_text_ctrl->SetMinSize(wxSize(241,33));
        //m_password_text_ctrl->SetValidator(wxTextValidator(wxFILTER_REGEX, "^[\w-]+(\.[\w-]+)*@[\w-]+(\.[\w-]+)+$"));

        //wxTextValidator validator(wxFILTER_INCLUDE_CHAR_LIST);
        //validator.SetCharIncludes("^[\w-]+(\.[\w-]+)*@[\w-]+(\.[\w-]+)+$");
        //m_password_text_ctrl->SetValidator(validator);
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

    void PasswordTextCtrl::OnShowPasswordButtonClicked(wxMouseEvent& event)
    {
        if (m_password_text_ctrl->GetWindowStyleFlag() & wxTE_PASSWORD) {
            wxString password = m_password_text_ctrl->GetValue();
            // 如果密码输入框当前为密码模式，则切换为明文模式
            //m_password_text_ctrl->SetWindowStyleFlag(wxTE_PROCESS_ENTER);
            m_password_text_ctrl->SetWindowStyle(m_password_text_ctrl->GetWindowStyle() & ~wxTE_PASSWORD);
            //m_password_text_ctrl->SetValue(m_password_text_ctrl->GetValue());
            m_password_text_ctrl->SetValue("");
            m_password_text_ctrl->SetValue("8888");
            m_showPassword_staticbitmap->SetBitmap(m_eye_on_bitmap);
            //m_showPassword_staticbitmap->SetToolTip(wxT("Hide password"));
            m_password_text_ctrl->Refresh();
            m_password_text_ctrl->Update();
        } else {
            // 如果密码输入框当前为明文模式，则切换为密码模式
            //m_password_text_ctrl->SetWindowStyleFlag(wxTE_PASSWORD);
            //m_password_text_ctrl->SetWindowStyle(wxTE_PASSWORD);
            m_password_text_ctrl->SetValue(m_password_text_ctrl->GetValue());
            m_showPassword_staticbitmap->SetBitmap(m_eye_off_bitmap);
            //m_showPassword_staticbitmap->SetToolTip(wxT("Show password"));
            m_password_text_ctrl->Refresh();
            m_password_text_ctrl->Update();
        }
    }

LoginDialog::LoginDialog()
    : DPIDialog(static_cast<wxWindow *>(wxGetApp().mainframe),wxID_ANY,from_u8((boost::format(_utf8(L("Login")))).str()),wxDefaultPosition,
        wxDefaultSize, /*wxCAPTION*/wxDEFAULT_DIALOG_STYLE)
{
    SetFont(wxGetApp().normal_font());
	SetBackgroundColour(*wxWHITE);
    //Centre(wxBOTH);

    //std::string icon_path = (boost::format("%1%/images/OrcaSlicerTitle.ico") % resources_dir()).str();
    //SetIcon(wxIcon(encode_path(icon_path.c_str()), wxBITMAP_TYPE_ICO));
 
    wxSizer* sizer_frame = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // 创建第一个标签页
    wxPanel* page1 = new wxPanel(this, wxID_ANY);
    wxBoxSizer* page1Sizer = new wxBoxSizer(wxVERTICAL);
    setupLayoutPage1(page1Sizer,page1);
    page1->SetSizer(page1Sizer);

    // 创建第二个标签页
    wxPanel* page2 = new wxPanel(this, wxID_ANY);
    wxBoxSizer* page2Sizer = new wxBoxSizer(wxVERTICAL);
    setupLayoutPage2(page2Sizer,page2);
    page2->SetSizer(page2Sizer);

    // 添加 wxPanel 控件
    sizer->Add(page1, 1, wxEXPAND | wxALL, 10);
    sizer->Add(page2, 1, wxEXPAND | wxALL, 10);
    page2->Hide(); 

    // 创建 wxStaticText 控件
    wxStaticText* label1 = new wxStaticText(this, wxID_ANY, "Verify Code Login/ Register");
    label1->SetForegroundColour(wxColour(51,51,51));
    wxStaticText* label2 = new wxStaticText(this, wxID_ANY, "Password Login");
    label2->SetForegroundColour(wxColour(153,153,153));
    
    // 绑定事件处理函数
    label1->Bind(wxEVT_LEFT_DOWN, [this,page1,page2,label2,label1](wxMouseEvent& e){
        page1->Show();page2->Hide();
        label1->SetForegroundColour(wxColour(51,51,51));
        label2->SetForegroundColour(wxColour(153,153,153));
        label1->Refresh();
        label2->Refresh();
        m_staticLine_verify->Show();
        m_staticLine_verify->Refresh();
        m_staticLine_password->Hide();
        m_staticLine_password->Refresh();
        Layout();
        });
    label2->Bind(wxEVT_LEFT_DOWN, [this,page1,page2,label1,label2](wxMouseEvent& e){
        page2->Show();page1->Hide();
        label2->SetForegroundColour(wxColour(51,51,51));
        label1->SetForegroundColour(wxColour(153,153,153));
        label1->Refresh();
        label2->Refresh();
        m_login_check_box_page2->Refresh();
        m_protocol_page2->Refresh();
        m_service_link_page2->Refresh();
        m_privacy_policy_link_page2->Refresh();
        m_staticLine_verify->Hide();
        m_staticLine_verify->Refresh();
        m_staticLine_password->Show();
        m_staticLine_password->Refresh();
        Layout();
        });

    //add line
   wxBoxSizer* verify_line_sizer = new wxBoxSizer(wxVERTICAL);

   m_staticLine_verify = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(90, 4));
   m_staticLine_verify->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
   m_staticLine_verify->SetBackgroundColour(wxColour(255, 255, 255));
   m_staticLine_verify->SetForegroundColour((wxColour(50,141,251)));
   verify_line_sizer->Add(label1, 1, wxALL |wxALIGN_CENTER,0);
   verify_line_sizer->Add(m_staticLine_verify, 0, wxALL |wxALIGN_CENTER, 0);

   wxBoxSizer* password_line_sizer = new wxBoxSizer(wxVERTICAL);

   
   m_staticLine_password = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(90, 4));
   m_staticLine_password->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
   m_staticLine_password->SetBackgroundColour(wxColour(255, 255, 255));
   m_staticLine_password->SetForegroundColour((wxColour(50,141,251)));
   m_staticLine_password->Hide();
   password_line_sizer->Add(label2, 1, wxALL |wxALIGN_CENTER,0);
   password_line_sizer->Add(m_staticLine_password, 0, wxALL |wxALIGN_CENTER, 0);

    // 创建 wxBoxSizer 布局管理器并添加 wxStaticText 控件
    wxBoxSizer* labelSizer = new wxBoxSizer(wxHORIZONTAL);
    //labelSizer->Add(label1, 0, wxALIGN_CENTER);
    labelSizer->Add(verify_line_sizer, 0, wxALIGN_CENTER);
    labelSizer->AddSpacer(FromDIP(33));
    //labelSizer->Add(label2, 0, wxALIGN_CENTER);
    labelSizer->Add(password_line_sizer, 0, wxALIGN_CENTER);

    // 添加 wxStaticText 控件和 wxPanel 控件
    sizer_frame->AddSpacer(FromDIP(53));
    sizer_frame->Add(labelSizer, 0, wxALIGN_CENTER);
    sizer_frame->Add(sizer, 0, wxALIGN_CENTER);

    SetSizerAndFit(sizer_frame);
}

void LoginDialog::OnUpdateText(wxCommandEvent& event)
{
    wxWindow* focusWindow = wxWindow::FindFocus();
    if(focusWindow != nullptr){
            SetFocus();
    }
    if(m_get_code_button){
        m_get_code_button->SetLabel(event.GetString());
        if(event.GetString() == "1"){
            verify_btn_clicked = true;
        }
    }
    event.Skip();
}

void LoginDialog::on_dpi_changed(const wxRect &suggested_rect)
{
    const int& em = em_unit();
    const wxSize& size = wxSize(65 * em, 500 * em);
    SetMinSize(size);
    Fit();
    Refresh();
}

void LoginDialog::setupLayoutPage1(wxBoxSizer* page1Sizer,wxPanel* parent)
{
    page1Sizer->Add(FromDIP(40), 0, 0, FromDIP(52));
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    //phone number / email
    wxBitmap usr_pic = create_scaled_bitmap("login-usr", this, 18);

    //left space
    wxPanel* usr_name_space1 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTAB_TRAVERSAL);
    usr_name_space1->SetMinSize(wxSize(66, -1));
    //right space
    wxPanel* usr_name_space2 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTAB_TRAVERSAL);
    usr_name_space2->SetMinSize(wxSize(86, -1));

    m_usrname_page1 = new UsrnameTextCtrl(usr_pic,parent,wxID_ANY);
    m_usrname_page1->Bind(wxEVT_TEXT, &LoginDialog::onUsrNameOrPasswordChangedPage1, this);
    wxStaticBoxSizer* border_sizer = new wxStaticBoxSizer(wxVERTICAL, parent, wxEmptyString);
    border_sizer->Add(m_usrname_page1);
    //add border
    border_sizer->GetStaticBox()->SetSizeHints(-1, -1, -1, 100);

    //adjust layout
    wxBoxSizer *last_sizer = new wxBoxSizer(wxHORIZONTAL);
    last_sizer->Add(usr_name_space1, 0, wxEXPAND|wxLeft, 0);
    last_sizer->Add(border_sizer, 0, wxALL, 0);
    last_sizer->Add(usr_name_space2, 0, wxEXPAND ,0);

    page1Sizer->Add(0,FromDIP(23),0, 0);
    page1Sizer->Add(last_sizer, 0, wxEXPAND ,0);

//*******verify code******** 

    // bitmap
    wxBitmap verify_pic = create_scaled_bitmap("login-verify", this, 18);

    wxStaticBoxSizer* verify_border_sizer = new wxStaticBoxSizer(wxVERTICAL, parent, wxEmptyString);
    verify_border_sizer->SetMinSize(wxSize(190,45));

    m_verify_code = new VerifycodeTextCtrl(verify_pic,parent,wxID_ANY);
    m_verify_code->Bind(wxEVT_TEXT, &LoginDialog::onUsrNameOrPasswordChangedPage1, this);
    verify_border_sizer->Add(m_verify_code);
    //add border
    verify_border_sizer->GetStaticBox()->SetSizeHints(-1, -1, -1, 100);

    m_get_code_button = new CountdownButton(parent,"Get Code");
    m_get_code_button->SetForegroundColour(wxColour(255, 255, 255));// 设置字体颜色
    m_get_code_button->SetBackgroundColour(wxColour(221,221,221)); // 设置背景色
    m_get_code_button->SetWindowStyleFlag(wxBORDER_NONE); //去除边框线
    m_get_code_button->SetMinSize(wxSize(114,54));
    m_get_code_button->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event){
        m_get_code_button->startTimer();
        //m_get_code_button->OnButtonClick(event);
    });
    //Bind(EVT_UPDATE_TEXT_LOGIN, &LoginDialog::OnUpdateText, this);


    //adjust layout
    wxBoxSizer *verify_last_sizer = new wxBoxSizer(wxHORIZONTAL);
    //verify_last_sizer->SetMinSize(250,-1);

    verify_last_sizer->Add(usr_name_space1, 0, wxEXPAND|wxLeft, 0);
    verify_last_sizer->Add(verify_border_sizer);
    verify_last_sizer->Add(m_get_code_button,0, wxALL,10);
    verify_last_sizer->Add(usr_name_space2, 0, wxEXPAND ,0);

    page1Sizer->Add(verify_last_sizer, 0, wxEXPAND ,0);

//****error tips ***
    m_error_label = new wxStaticText(parent,wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    m_error_label->SetLabel("Verify code is incorrect");
    m_error_label->SetFont((wxFont(wxFontInfo(16))));
    m_error_label->SetBackgroundColour(wxColour(250, 207, 202)); // #FACFCA
    m_error_label->SetForegroundColour(wxColour(234, 53, 34)); // #EA3522
    m_error_label->SetWindowStyle(wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    m_error_label->SetMinSize(wxSize(348,55));

    page1Sizer->Add(m_error_label, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
    m_error_label->Show(false); 

    //login button
    m_login_button_page1 = new wxButton(parent, wxID_ANY, "Login");
    m_login_button_page1->SetForegroundColour(wxColour(255, 255, 255));
    m_login_button_page1->SetBackgroundColour(wxColour(221,221,221)); 
    m_login_button_page1->SetWindowStyleFlag(wxBORDER_NONE); 
    m_login_button_page1->SetMinSize(wxSize(101,44));
    m_login_button_page1->SetFont((wxFont(wxFontInfo(16))));

    page1Sizer->Add(m_login_button_page1, 0, wxALIGN_CENTER_HORIZONTAL| wxUP, FromDIP(36));

    //check box
    wxBoxSizer* checkbox_sizer = new wxBoxSizer(wxHORIZONTAL);

    m_login_check_box_page1 = new wxCheckBox(parent, wxID_ANY, _L(""), wxDefaultPosition, wxDefaultSize, 0);
    m_login_check_box_page1->Bind(wxEVT_CHECKBOX, &LoginDialog::onAgreeCheckBoxChangedPage1, this);

    wxStaticText* protocol = new  wxStaticText(parent, wxID_ANY,"Read and Agree to Accept");
    protocol->SetFont((wxFont(wxFontInfo(14))));

    //Service Item
    wxStaticText* service_link = new wxStaticText(parent, wxID_ANY, _L("《Term of Sevrvice》"));
    service_link->SetFont((wxFont(wxFontInfo(14))));
    //service_link->Wrap(100);

    service_link->Bind(wxEVT_LEFT_UP, [this,parent](wxMouseEvent& e){
         wxTextCtrl* textCtrl = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
         textCtrl->LoadFile("");
         wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
         sizer->Add(textCtrl, wxSizerFlags(1).Expand());

         wxDialog* dialog = new wxDialog(parent, wxID_ANY, "Term of Sevrvice", wxDefaultPosition, wxSize(400, 300));
         dialog->SetSizer(sizer);
         dialog->ShowModal();
    });

    //Privacy Policy
    wxStaticText* privacy_policy_link = new wxStaticText(parent, wxID_ANY, _L("《Privacy Policy》"));
    privacy_policy_link->SetFont((wxFont(wxFontInfo(14))));
    //privacy_policy_link->Wrap(100);

    privacy_policy_link->Bind(wxEVT_LEFT_UP, [this,parent](wxMouseEvent& e){
         wxTextCtrl* textCtrl = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
         textCtrl->LoadFile("");
         wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
         sizer->Add(textCtrl, wxSizerFlags(1).Expand());

         wxDialog* dialog = new wxDialog(parent, wxID_ANY, "Privacy Policy", wxDefaultPosition, wxSize(400, 300));
         dialog->SetSizer(sizer);
         dialog->ShowModal();
    });

    //left gaption
    checkbox_sizer->Add(FromDIP(50), 0, 0, 0);
    checkbox_sizer->Add(m_login_check_box_page1, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(5));
    checkbox_sizer->Add(protocol,0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(5));
    //checkbox_sizer->Add(service_link,0, wxTOP, FromDIP(5));
    //checkbox_sizer->Add(privacy_policy_link,0, wxTOP, FromDIP(5));

    page1Sizer->Add(checkbox_sizer, 0, wxEXPAND, 0);
    page1Sizer->Add(service_link, 0,  wxTOP, FromDIP(5));
    page1Sizer->Add(privacy_policy_link, 0,  wxTOP, FromDIP(5));
}

void LoginDialog::setupLayoutPage2(wxBoxSizer* page2Sizer,wxPanel* parent)
{
    page2Sizer->Add(FromDIP(40), 0, 0, FromDIP(52));
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    //phone number / email
    // bitmap
    wxBitmap usr_pic = create_scaled_bitmap("login-usr", this, 18);

    //left space
    wxPanel* usr_name_space1 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTAB_TRAVERSAL);
    usr_name_space1->SetMinSize(wxSize(66, -1));
    //right space
    wxPanel* usr_name_space2 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTAB_TRAVERSAL);
    usr_name_space2->SetMinSize(wxSize(86, -1));

    wxStaticBoxSizer* border_sizer = new wxStaticBoxSizer(wxVERTICAL, parent, wxEmptyString);

    m_usrname_page2 = new UsrnameTextCtrl(usr_pic,parent,wxID_ANY);
    m_usrname_page2->Bind(wxEVT_TEXT, &LoginDialog::onUsrNameOrPasswordChangedPage2, this);

    //add border
    border_sizer->Add(m_usrname_page2);
    border_sizer->GetStaticBox()->SetSizeHints(-1, -1, -1, 100);

    //adjust layout
    wxBoxSizer *last_sizer = new wxBoxSizer(wxHORIZONTAL);
    //last_sizer->SetMinSize(250,-1);

    last_sizer->Add(usr_name_space1, 0, wxEXPAND|wxLeft, 0);
    last_sizer->Add(border_sizer, 0, wxALL, 0);
    last_sizer->Add(usr_name_space2, 0, wxEXPAND ,0);

    page2Sizer->Add(0,FromDIP(23),0, 0);
    page2Sizer->Add(last_sizer, 0, wxEXPAND ,0);

//*******password******** 
    wxBitmap login_lock_pic = create_scaled_bitmap("login-lock", this, 18);
    wxBitmap login_eye_off_pic = create_scaled_bitmap("login-eye-off", this, 18);
    wxBitmap login_eye_on_pic = create_scaled_bitmap("login-eye-on", this, 18);

    m_password = new PasswordTextCtrl(login_lock_pic,login_eye_off_pic,login_eye_on_pic,parent,wxID_ANY);
    m_password->Bind(wxEVT_TEXT, &LoginDialog::onUsrNameOrPasswordChangedPage2, this);
    wxStaticBoxSizer* verify_border_sizer = new wxStaticBoxSizer(wxVERTICAL, parent, wxEmptyString);
    verify_border_sizer->SetMinSize(wxSize(190,45));
    verify_border_sizer->Add(m_password);
    //add border
    verify_border_sizer->GetStaticBox()->SetSizeHints(-1, -1, -1, 100);

    //adjust layout
    wxBoxSizer *verify_last_sizer = new wxBoxSizer(wxHORIZONTAL);
    verify_last_sizer->Add(usr_name_space1, 0, wxEXPAND|wxLeft, 0);
    verify_last_sizer->Add(verify_border_sizer);
    verify_last_sizer->Add(usr_name_space2, 0, wxEXPAND ,0);

    page2Sizer->Add(verify_last_sizer, 0, wxEXPAND ,0);

    //register / forget password
    wxHyperlinkCtrl* register_link = new wxHyperlinkCtrl(parent, wxID_ANY, "Register", wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    register_link->Bind(wxEVT_HYPERLINK, [this](wxCommandEvent& e){
        wxString url = "https://www.baidu.com/";
        wxLaunchDefaultBrowser(url);
    });

    wxHyperlinkCtrl* forget_password_link = new wxHyperlinkCtrl(parent, wxID_ANY, "Forget Password", wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    forget_password_link->Bind(wxEVT_HYPERLINK, [this](wxCommandEvent& e){
        wxString url = "https://www.youku.com/";
        wxLaunchDefaultBrowser(url);
    });

    wxBoxSizer *regist_forget_hor_sizer = new wxBoxSizer(wxHORIZONTAL);
    regist_forget_hor_sizer->SetMinSize(250,-1);

    regist_forget_hor_sizer->Add(usr_name_space1, 0, wxEXPAND|wxLeft, 0);
    regist_forget_hor_sizer->Add(register_link, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(10));
    regist_forget_hor_sizer->AddSpacer(135);
    regist_forget_hor_sizer->Add(forget_password_link, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(10));

    page2Sizer->Add(regist_forget_hor_sizer, 0, wxEXPAND | wxALIGN_CENTER_HORIZONTAL,0);

    //login button
    m_login_button_page2 = new wxButton(parent, wxID_ANY, "Login");
    m_login_button_page2->SetMinSize(wxSize(101,44));
    m_login_button_page2->SetFont((wxFont(wxFontInfo(16))));
    m_login_button_page2->SetForegroundColour(wxColour(255, 255, 255));
    m_login_button_page2->SetBackgroundColour(wxColour(221,221,221)); 
    m_login_button_page2->SetWindowStyleFlag(wxBORDER_NONE); 

    page2Sizer->Add(m_login_button_page2, 0, wxALIGN_CENTER_HORIZONTAL | wxUP, FromDIP(30));

    //check box
    wxBoxSizer* checkbox_sizer = new wxBoxSizer(wxHORIZONTAL);

    m_login_check_box_page2 = new wxCheckBox(parent, wxID_ANY, _L(""), wxDefaultPosition, wxDefaultSize, 0);
    // m_login_check_box_page2->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, [this](wxCommandEvent& e) {
    // });
    m_login_check_box_page2->Bind(wxEVT_CHECKBOX, &LoginDialog::onAgreeCheckBoxChangedPage2, this);

    m_protocol_page2 = new  wxStaticText(parent, wxID_ANY,"Read and Agree to Accept");
    m_protocol_page2->SetFont((wxFont(wxFontInfo(14))));
    //protocol->SetMinSize(wxSize(46,28));

    //Service Item
    m_service_link_page2 = new wxStaticText(parent, wxID_ANY, _L("《Term of Sevrvice》"));
    m_service_link_page2->SetFont((wxFont(wxFontInfo(14))));

    m_service_link_page2->Bind(wxEVT_LEFT_UP, [this,parent](wxMouseEvent& e){
         wxTextCtrl* textCtrl = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
         textCtrl->LoadFile("");
         wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
         sizer->Add(textCtrl, wxSizerFlags(1).Expand());

         wxDialog* dialog = new wxDialog(parent, wxID_ANY, "Term of Sevrvice", wxDefaultPosition, wxSize(400, 300));
         dialog->SetSizer(sizer);
         dialog->ShowModal();
    });

    //Privacy Policy
    m_privacy_policy_link_page2 = new wxStaticText(parent, wxID_ANY, _L("《Privacy Policy》"));
    m_privacy_policy_link_page2->SetFont((wxFont(wxFontInfo(14))));
    m_privacy_policy_link_page2->Bind(wxEVT_LEFT_UP, [this,parent](wxMouseEvent& e){
         wxTextCtrl* textCtrl = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
         textCtrl->LoadFile("");
         wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
         sizer->Add(textCtrl, wxSizerFlags(1).Expand());

         wxDialog* dialog = new wxDialog(parent, wxID_ANY, "Privacy Policy", wxDefaultPosition, wxSize(400, 300));
         dialog->SetSizer(sizer);
         dialog->ShowModal();
    });

    //left gaption
    checkbox_sizer->Add(FromDIP(50), 0, 0, 0);
    checkbox_sizer->Add(m_login_check_box_page2, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(5));
    checkbox_sizer->Add(m_protocol_page2,0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(5));
    //checkbox_sizer->Add(m_service_link_page2,0, wxTOP, FromDIP(5));
    //checkbox_sizer->Add(m_privacy_policy_link_page2,0, wxTOP, FromDIP(5));

    page2Sizer->Add(checkbox_sizer, 0, wxEXPAND, 0);
    page2Sizer->Add(m_service_link_page2, 0,  wxTOP, FromDIP(5));
    page2Sizer->Add(m_privacy_policy_link_page2, 0,  wxTOP, FromDIP(5));
}

void LoginDialog::onUsrNameOrPasswordChangedPage1(wxCommandEvent& event)
{
    //
        wxString username = m_usrname_page1->GetValue();
        wxString verifycode = m_verify_code->GetValue();
        bool agree = m_login_check_box_page1->GetValue();
        if (!username.IsEmpty() && !verifycode.IsEmpty() && agree)
        {
            m_login_button_page1->Enable();
            m_login_button_page1->SetForegroundColour(wxColour(255, 255, 255));
            m_login_button_page1->SetBackgroundColour(wxColour(50,141,251)); 
        }
        else{
            m_login_button_page1->Disable();
            m_login_button_page1->SetForegroundColour(wxColour(255, 255, 255));
            m_login_button_page1->SetBackgroundColour(wxColour(221,221,221)); 
        }
}

void LoginDialog::onAgreeCheckBoxChangedPage1(wxCommandEvent& event)
{
        wxString username = m_usrname_page1->GetValue();
        wxString verifycode = m_verify_code->GetValue();
        bool agree = m_login_check_box_page1->GetValue();
        if (!username.IsEmpty() && !verifycode.IsEmpty() && agree)
        {
            m_login_button_page1->Enable();
            m_login_button_page1->SetForegroundColour(wxColour(255, 255, 255));
            m_login_button_page1->SetBackgroundColour(wxColour(50,141,251)); 
        }
        else{
            m_login_button_page1->Disable();
            m_login_button_page1->SetForegroundColour(wxColour(255, 255, 255));
            m_login_button_page1->SetBackgroundColour(wxColour(221,221,221)); 
        }
}

void LoginDialog::onUsrNameOrPasswordChangedPage2(wxCommandEvent& event)
{
        wxString username = m_usrname_page2->GetValue();
        wxString password = m_password->GetValue();
        bool agree = m_login_check_box_page2->GetValue();
        if (!username.IsEmpty() && !password.IsEmpty() && agree)
        {
            m_login_button_page2->Enable();
            m_login_button_page2->SetForegroundColour(wxColour(255, 255, 255));
            m_login_button_page2->SetBackgroundColour(wxColour(50,141,251));
        }
        else{
            m_login_button_page2->Disable();
            m_login_button_page2->SetForegroundColour(wxColour(255, 255, 255));
            m_login_button_page2->SetBackgroundColour(wxColour(221,221,221)); 
        }
}

void LoginDialog::onAgreeCheckBoxChangedPage2(wxCommandEvent& event)
{
        wxString username = m_usrname_page2->GetValue();
        wxString password = m_password->GetValue();
        bool agree = m_login_check_box_page2->GetValue();
        if (!username.IsEmpty() && !password.IsEmpty() && agree)
        {
            m_login_button_page2->Enable();
            m_login_button_page2->SetForegroundColour(wxColour(255, 255, 255));
            m_login_button_page2->SetBackgroundColour(wxColour(50,141,251)); 
        }
        else{
            m_login_button_page2->Disable();
            m_login_button_page2->SetForegroundColour(wxColour(255, 255, 255));
            m_login_button_page2->SetBackgroundColour(wxColour(221,221,221)); 
        }
}


}
}