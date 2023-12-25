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

private:
    void OnShowPasswordButtonClicked(wxMouseEvent& event)
    {
        if (m_password_text_ctrl->GetWindowStyleFlag() & wxTE_PASSWORD) {
            // 如果密码输入框当前为密码模式，则切换为明文模式
            m_password_text_ctrl->SetWindowStyleFlag(wxTE_PROCESS_ENTER);
            m_password_text_ctrl->SetValue(m_password_text_ctrl->GetValue());
            m_showPassword_staticbitmap->SetBitmap(m_eye_on_bitmap);
            m_showPassword_staticbitmap->SetToolTip(wxT("Hide password"));
        } else {
            // 如果密码输入框当前为明文模式，则切换为密码模式
            m_password_text_ctrl->SetWindowStyleFlag(wxTE_PASSWORD);
            m_password_text_ctrl->SetValue(m_password_text_ctrl->GetValue());
            m_showPassword_staticbitmap->SetBitmap(m_eye_off_bitmap);
            m_showPassword_staticbitmap->SetToolTip(wxT("Show password"));
        }
    }

private:
    wxTextCtrl*      m_password_text_ctrl;
    wxBitmap         m_eye_off_bitmap;
    wxBitmap         m_eye_on_bitmap;
    wxStaticBitmap*  m_lock_staticbitmap;
    wxStaticBitmap*  m_showPassword_staticbitmap;
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

private:
    wxTextCtrl*      m_user_name_text_ctrl;
    wxStaticBitmap*  m_usr_staticbitmap;
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

private:
    wxTextCtrl*      m_verify_code_text_ctrl;
    wxStaticBitmap*  m_verify_staticbitmap;
};

class MyNotebook : public wxNotebook
{
public:
    MyNotebook(wxWindow* parent, 
               wxWindowID id,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxString& name = wxASCII_STR(wxNotebookNameStr))
        : wxNotebook(parent, id,pos,size,wxNB_NOPAGETHEME,name)
    {
        // 将选项卡的内容居中显示
        //SetWindowStyle(GetWindowStyle() & ~wxNB_LEFT & ~wxNB_RIGHT);
    }

};

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

    // 创建 wxNotebook 对象
    //wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    MyNotebook* notebook = new MyNotebook(this, wxID_ANY);
    //notebook->SetPadding(wxSize(30,35));

    // 创建第一个标签页
    wxPanel* page1 = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer* page1Sizer = new wxBoxSizer(wxVERTICAL);
    setupLayoutPage1(page1Sizer,page1);
    page1->SetSizer(page1Sizer);
    notebook->AddPage(page1, "Verify Code Login/ Register");

    // 创建第二个标签页
    wxPanel* page2 = new wxPanel(notebook, wxID_ANY);
    wxBoxSizer* page2Sizer = new wxBoxSizer(wxVERTICAL);
    setupLayoutPage2(page2Sizer,page2);
    page2->SetSizer(page2Sizer);
    notebook->AddPage(page2, "Password Login");

    // 将 wxNotebook 控件添加到 sizer 中
    //sizer->Add(notebook, 1, wxEXPAND | wxALL |wxALIGN_CENTER_HORIZONTAL, 5);

    //notebook->Layout();

    sizer->Add(notebook, 0, wxALIGN_CENTER | wxALL, 0);

    sizer_frame->Add(sizer, 0, wxTOP, FromDIP(53));
    SetSizerAndFit(sizer_frame);

/*
    //登录标题
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Login");
    title->SetFont(wxFont(wxFontInfo(24)));
    sizer->Add(title, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(30));

//***********************

    //手机号标签
    wxStaticText* phone_num_label = new wxStaticText(this, wxID_ANY, "Phone Number");
    phone_num_label->SetFont(wxFont(wxFontInfo(16)));

    //添加空的间距
    //左侧间距
    wxPanel* space1 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    space1->SetMinSize(wxSize(80, -1));
    //右侧间距
    wxPanel* space2 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    space2->SetMinSize(wxSize(80, -1));

    wxBoxSizer *phone_number_hor_sizer = new wxBoxSizer(wxHORIZONTAL);
    phone_number_hor_sizer->SetMinSize(250,-1);

    phone_number_hor_sizer->Add(space1, 0, wxEXPAND ,0);
    phone_number_hor_sizer->Add(phone_num_label,0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(10));
    phone_number_hor_sizer->Add(space2, 0, wxEXPAND ,0);

    sizer->Add(phone_number_hor_sizer, 0, wxEXPAND ,0);
    //sizer->Add(phone_num_label,0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(10));

//***********************

    //手机号
    wxStaticText* phone_86 = new  wxStaticText(this, wxID_ANY,"+86");
    phone_86->SetFont((wxFont(wxFontInfo(16))));
    phone_86->SetMinSize(wxSize(46,28));

    wxTextCtrl* username = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxBORDER_NONE);
    username->SetHint("Input Phone Number");
    username->SetMinSize(wxSize(274,28));

    //添加空的间距
    //左侧间距
    wxPanel* user_name_space = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    user_name_space->SetMinSize(wxSize(80, -1));
    //右侧间距
    wxPanel* user_name_space_2 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    user_name_space_2->SetMinSize(wxSize(80, -1));

    wxBoxSizer *user_name_hor_sizer = new wxBoxSizer(wxHORIZONTAL);
    user_name_hor_sizer->SetMinSize(250,-1);

    user_name_hor_sizer->Add(user_name_space, 0, wxEXPAND ,0);
    user_name_hor_sizer->Add(phone_86, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(10));
    user_name_hor_sizer->AddSpacer(20);
    user_name_hor_sizer->Add(username, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(10));
    user_name_hor_sizer->Add(user_name_space_2, 0, wxEXPAND ,0);

    sizer->Add(user_name_hor_sizer, 0, wxEXPAND ,0);
    //sizer->Add(username, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, FromDIP(30));

//***********************
    //分隔线
    wxStaticLine* line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(260), FromDIP(1)), wxLI_HORIZONTAL);

    wxBoxSizer* line_hor_sizer = new wxBoxSizer(wxVERTICAL);
    //line_hor_sizer->AddSpacer(80);
    line_hor_sizer->Add(line, 0, wxSTRETCH_NOT | wxLEFT | wxRIGHT,80);
    //line_hor_sizer->AddSpacer(80);

    sizer->Add(line_hor_sizer, 0, wxEXPAND ,0);
    //sizer->Add(line, 0, wxEXPAND | wxALL, 10);

//***********************

    //验证码标签
    wxStaticText* verifyCode = new  wxStaticText(this, wxID_ANY,"Verify Code");
    verifyCode->SetFont((wxFont(wxFontInfo(16))));

    //验证码添加空的间距
    //左侧间距
    wxPanel* space3 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    space3->SetMinSize(wxSize(80, -1));
    //右侧间距
    wxPanel* space4 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    space4->SetMinSize(wxSize(80, -1));

    wxBoxSizer *verify_code_hor_sizer = new wxBoxSizer(wxHORIZONTAL);
    verify_code_hor_sizer->SetMinSize(250,-1);

    verify_code_hor_sizer->Add(space3, 0, wxEXPAND ,0);
    verify_code_hor_sizer->Add(verifyCode,0,wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(5));
    verify_code_hor_sizer->Add(space4, 0, wxEXPAND ,0);

    sizer->Add(verify_code_hor_sizer, 0, wxEXPAND ,0);
    //sizer->Add(verifyCode,0,wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(5));

//***********************

    //验证码    

    wxTextCtrl* password = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxBORDER_NONE);
    password->SetHint("Input Verify Code");
    password->SetMinSize(wxSize(79,28));

    wxStaticText* send_verify_code = new  wxStaticText(this, wxID_ANY,"Send Verify Code");
    send_verify_code->SetFont((wxFont(wxFontInfo(16))));
    send_verify_code->SetMinSize(wxSize(79,28));

    //添加空的间距
    //左侧间距
    wxPanel* password_space = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    password_space->SetMinSize(wxSize(80, -1));
    //右侧间距
    wxPanel* password_space_2 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    password_space_2->SetMinSize(wxSize(80, -1));

    wxBoxSizer *password_hor_sizer = new wxBoxSizer(wxHORIZONTAL);
    password_hor_sizer->SetMinSize(250,-1);

    password_hor_sizer->Add(password_space, 0, wxEXPAND ,0);
    password_hor_sizer->Add(password, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(10));
    password_hor_sizer->AddSpacer(182);
    password_hor_sizer->Add(send_verify_code, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(10));
    password_hor_sizer->Add(password_space_2, 0, wxEXPAND ,0);

    sizer->Add(password_hor_sizer, 0, wxEXPAND ,0);
    //sizer->Add(password, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

//***********************

    //分隔线
    wxStaticLine* line_2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(260), FromDIP(1)), wxLI_HORIZONTAL);
    wxBoxSizer* line_hor_sizer_2 = new wxBoxSizer(wxVERTICAL);
    //line_hor_sizer_2->AddSpacer(80);
    line_hor_sizer_2->Add(line_2, 0, wxSTRETCH_NOT | wxLEFT | wxRIGHT,80);
    //line_hor_sizer_2->AddSpacer(80);

    sizer->Add(line_hor_sizer_2, 0, wxEXPAND ,0);
    //sizer->Add(line_2, 0, wxEXPAND | wxALL, 10);

//***********************

    //使用账号密码登录
    wxStaticText* account_password_login = new  wxStaticText(this, wxID_ANY,"Login with Account and Password");
    account_password_login->SetFont((wxFont(wxFontInfo(12))));
    account_password_login->SetMinSize(wxSize(180,28));

    wxPanel* account_password_space = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    account_password_space->SetMinSize(wxSize(310, -1));
    //右侧间距
    wxPanel* account_password_space_2 = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    account_password_space_2->SetMinSize(wxSize(80, -1));

    wxBoxSizer *account_password_hor_sizer = new wxBoxSizer(wxHORIZONTAL);
    account_password_hor_sizer->SetMinSize(250,-1);

    //account_password_hor_sizer->Add(account_password_space, 0, wxEXPAND ,0);
    account_password_hor_sizer->Add(FromDIP(185),0,0,0);
    account_password_hor_sizer->Add(account_password_login, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(10));
    account_password_hor_sizer->Add(0,0,0,FromDIP(50));
    //account_password_hor_sizer->Add(account_password_space_2, 0, wxEXPAND ,0);

    sizer->Add(account_password_hor_sizer, 0, wxEXPAND ,0);

//***********************
    //登录按钮
    wxButton* login_button = new wxButton(this, wxID_ANY, "Login");
    login_button->SetMinSize(wxSize(101,44));

    sizer->Add(login_button, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    // wxButton* cancelButton = new wxButton(this, wxID_ANY, "Cancel");
    // sizer->Add(cancelButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

//***********************
    //勾选框
    auto checkbox_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxCheckBox* login_check_box = new wxCheckBox(this, wxID_ANY, _L(""), wxDefaultPosition, wxDefaultSize, 0);
    login_check_box->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, [this](wxCommandEvent& e) {

    });

    wxStaticText* protocol = new  wxStaticText(this, wxID_ANY,"Read and Agree to Accept");
    protocol->SetFont((wxFont(wxFontInfo(14))));
    //protocol->SetMinSize(wxSize(46,28));

    //服务条款
    wxHyperlinkCtrl* link = new wxHyperlinkCtrl(this, wxID_ANY, "《Term of Sevrvice》", wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE);
    link->Bind(wxEVT_HYPERLINK, [this](wxCommandEvent& e){
         wxTextCtrl* textCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
         textCtrl->LoadFile("");
         wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
         sizer->Add(textCtrl, wxSizerFlags(1).Expand());

         wxDialog* dialog = new wxDialog(this, wxID_ANY, "Term of Sevrvice", wxDefaultPosition, wxSize(400, 300));
         dialog->SetSizer(sizer);
         dialog->ShowModal();
    });

    //左间距
    checkbox_sizer->Add(FromDIP(50), 0, 0, 0);
    checkbox_sizer->Add(login_check_box, 0, wxALL, FromDIP(5));
    //checkbox_sizer->AddSpacer(FromDIP(6));
    checkbox_sizer->Add(protocol,0, wxALL, FromDIP(5));


    sizer->Add(checkbox_sizer, 0, wxBOTTOM | wxEXPAND, 0);

    SetSizer(sizer);

    Layout();
    Fit();
    CenterOnParent();*/
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

    UsrnameTextCtrl* usrname = new UsrnameTextCtrl(usr_pic,parent,wxID_ANY);
    wxStaticBoxSizer* border_sizer = new wxStaticBoxSizer(wxVERTICAL, parent, wxEmptyString);
    border_sizer->Add(usrname);
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

    VerifycodeTextCtrl* verifycode = new VerifycodeTextCtrl(verify_pic,parent,wxID_ANY);
    verify_border_sizer->Add(verifycode);
    //add border
    verify_border_sizer->GetStaticBox()->SetSizeHints(-1, -1, -1, 100);

    CountdownButton* get_code_button = new CountdownButton(parent, wxID_ANY, "Get Code");
    get_code_button->SetMinSize(wxSize(114,54));
    get_code_button->Bind(wxEVT_LEFT_UP, [this,get_code_button](wxMouseEvent& e){
        get_code_button->startTimer();
    });


    //adjust layout
    wxBoxSizer *verify_last_sizer = new wxBoxSizer(wxHORIZONTAL);
    //verify_last_sizer->SetMinSize(250,-1);

    verify_last_sizer->Add(usr_name_space1, 0, wxEXPAND|wxLeft, 0);
    verify_last_sizer->Add(verify_border_sizer);
    verify_last_sizer->Add(get_code_button,0,wxALL,10);
    verify_last_sizer->Add(usr_name_space2, 0, wxEXPAND ,0);

    page1Sizer->Add(verify_last_sizer, 0, wxEXPAND ,0);

//****error tips ***
    wxStaticText* errorLabel = new wxStaticText(parent,wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    errorLabel->SetLabel("Verify code is incorrect");
    errorLabel->SetMinSize(wxSize(348,55)); 
    errorLabel->SetFont((wxFont(wxFontInfo(16))));
    errorLabel->SetBackgroundColour(wxColour(250, 207, 202)); // #FACFCA
    errorLabel->SetForegroundColour(wxColour(234, 53, 34)); // #EA3522

    page1Sizer->Add(errorLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxUP, 16);

    //login button
    wxButton* login_button = new wxButton(parent, wxID_ANY, "Login");
    login_button->SetMinSize(wxSize(101,44));
    login_button->SetFont((wxFont(wxFontInfo(16))));

    //page1Sizer->Add(login_button, 0, wxALIGN_CENTER_HORIZONTAL | wxUP, 60);
    page1Sizer->Add(login_button, 0, wxALIGN_CENTER_HORIZONTAL);

    //check box
    wxBoxSizer* checkbox_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxCheckBox* login_check_box = new wxCheckBox(parent, wxID_ANY, _L(""), wxDefaultPosition, wxDefaultSize, 0);
    login_check_box->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, [this](wxCommandEvent& e) {

    });

    wxStaticText* protocol = new  wxStaticText(parent, wxID_ANY,"Read and Agree to Accept");
    protocol->SetFont((wxFont(wxFontInfo(14))));
    //protocol->SetMinSize(wxSize(46,28));

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
    checkbox_sizer->Add(login_check_box, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(5));
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

    UsrnameTextCtrl* usrname = new UsrnameTextCtrl(usr_pic,parent,wxID_ANY);

    //add border
    border_sizer->Add(usrname);
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

    PasswordTextCtrl* password = new PasswordTextCtrl(login_lock_pic,login_eye_off_pic,login_eye_on_pic,parent,wxID_ANY);
    wxStaticBoxSizer* verify_border_sizer = new wxStaticBoxSizer(wxVERTICAL, parent, wxEmptyString);
    verify_border_sizer->SetMinSize(wxSize(190,45));
    verify_border_sizer->Add(password);
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
    wxButton* login_button = new wxButton(parent, wxID_ANY, "Login");
    login_button->SetMinSize(wxSize(101,44));
    login_button->SetFont((wxFont(wxFontInfo(16))));

    page2Sizer->Add(login_button, 0, wxALIGN_CENTER_HORIZONTAL | wxUP, 60);

    //check box
    wxBoxSizer* checkbox_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxCheckBox* login_check_box = new wxCheckBox(parent, wxID_ANY, _L(""), wxDefaultPosition, wxDefaultSize, 0);
    login_check_box->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, [this](wxCommandEvent& e) {

    });

    wxStaticText* protocol = new  wxStaticText(parent, wxID_ANY,"Read and Agree to Accept");
    protocol->SetFont((wxFont(wxFontInfo(14))));
    //protocol->SetMinSize(wxSize(46,28));

    //Service Item
    wxStaticText* service_link = new wxStaticText(parent, wxID_ANY, _L("《Term of Sevrvice》"));
    service_link->SetFont((wxFont(wxFontInfo(14))));

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
    checkbox_sizer->Add(login_check_box, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(5));
    checkbox_sizer->Add(protocol,0, wxALIGN_CENTER_HORIZONTAL | wxTOP, FromDIP(5));
    //checkbox_sizer->Add(service_link,0, wxTOP, FromDIP(5));
    //checkbox_sizer->Add(privacy_policy_link,0, wxTOP, FromDIP(5));

    page2Sizer->Add(checkbox_sizer, 0, wxEXPAND, 0);
    page2Sizer->Add(service_link, 0,  wxTOP, FromDIP(5));
    page2Sizer->Add(privacy_policy_link, 0,  wxTOP, FromDIP(5));
}



}
}