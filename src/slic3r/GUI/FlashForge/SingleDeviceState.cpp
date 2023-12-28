#include "SingleDeviceState.hpp"
#include "slic3r/GUI/BitmapCache.hpp"

namespace Slic3r {
namespace GUI {

MaterialPanel::MaterialPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY,wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
    {
        SetBackgroundColour(*wxWHITE);
        create_panel(this);
    }

MaterialPanel::~MaterialPanel()
{

}

void MaterialPanel::create_panel(wxWindow* parent)
{
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer *bSizer_printing_title = new wxBoxSizer(wxHORIZONTAL);

        m_panel_printing_title = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(36)), wxTAB_TRAVERSAL);
        m_panel_printing_title->SetBackgroundColour(wxColour(248,248,248));

        //材料站标题
        m_staticText_printing = new wxStaticText(m_panel_printing_title, wxID_ANY ,("Material Station"));
        m_staticText_printing->Wrap(-1);
        m_staticText_printing->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_printing->SetForegroundColour(wxColour(51,51,51));

        bSizer_printing_title->Add(m_staticText_printing, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, FromDIP(17));
        bSizer_printing_title->Add(0, 0, 1, wxEXPAND, 0);
        m_panel_printing_title->SetSizer(bSizer_printing_title);
        m_panel_printing_title->Layout();
        bSizer_printing_title->Fit(m_panel_printing_title);

        //材料站内容
        wxBoxSizer *bSizer_task_name_hor = new wxBoxSizer(wxHORIZONTAL);
        wxPanel*    task_name_panel      = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(108)), wxTAB_TRAVERSAL);
        m_staticText_subtask_value = new wxStaticText(task_name_panel, wxID_ANY, ("Unconnected"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER | wxST_ELLIPSIZE_END);
        m_staticText_subtask_value->Wrap(-1);
        m_staticText_subtask_value->SetFont(wxFont(wxFontInfo(16)));

        bSizer_task_name_hor->Add(m_staticText_subtask_value, 1, wxALL | wxEXPAND | wxALIGN_CENTER, 0);

        task_name_panel->SetSizer(bSizer_task_name_hor);
        task_name_panel->Layout();
        bSizer_task_name_hor->Fit(task_name_panel);

        sizer->Add(m_panel_printing_title, 0, wxEXPAND | wxALL, 0);
        sizer->Add(0, FromDIP(12), 0);
        sizer->Add(task_name_panel, 0, wxEXPAND | wxALIGN_CENTER, 0);

        parent->SetSizer(sizer);
        parent->Layout();
        parent->Fit();

}

SingleDeviceState::SingleDeviceState(wxWindow* parent, wxWindowID id, const wxPoint& pos, 
        const wxSize& size, long style, const wxString& name)
        : wxScrolledWindow(parent, id, pos, size, wxHSCROLL | wxVSCROLL)
{
        this->SetScrollRate(5, 5);
        this->SetBackgroundColour(wxColour(240,240,240));

        //最外层框架布局
        wxBoxSizer *bSizer_status = new wxBoxSizer(wxVERTICAL);

        //顶部背景条（作为间距）
        auto m_panel_separotor_top_back = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(18)), wxTAB_TRAVERSAL);
        m_panel_separotor_top_back->SetBackgroundColour(wxColour(240,240,240));
        bSizer_status->Add(m_panel_separotor_top_back, 0, wxEXPAND | wxALL, 0);

        //第二段水平布局
        wxBoxSizer *bSizer_status_below = new wxBoxSizer(wxHORIZONTAL);
        //第二段水平布局，左侧空白
        auto m_panel_separotor_left = new wxPanel(this, wxID_ANY, wxDefaultPosition,wxDefaultSize, wxTAB_TRAVERSAL);
        m_panel_separotor_left->SetBackgroundColour(wxColour(240, 240, 240));
        m_panel_separotor_left->SetMinSize(wxSize(FromDIP(10), -1));

        bSizer_status_below->Add(m_panel_separotor_left, 0, wxEXPAND | wxALL, 0);
        
        //第二段水平布局，中间垂直布局
        wxBoxSizer *bSizer_left = new wxBoxSizer(wxVERTICAL);

        //第二段水平布局，相机布局
        auto m_monitoring_sizer = create_monitoring_page();
        bSizer_left->Add(m_monitoring_sizer, 1, wxEXPAND | wxALL, 0);

        //第二段水平布局，相机垂直布局的中间间隔
        auto m_panel_separotor1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        m_panel_separotor1->SetBackgroundColour(wxColour(240, 240, 240));
        m_panel_separotor1->SetMinSize(wxSize(-1, FromDIP(10)));
        m_panel_separotor1->SetMaxSize(wxSize(-1, FromDIP(10)));
        m_monitoring_sizer->Add(m_panel_separotor1, 0, wxEXPAND, 0);

        //第二段水平布局，相机垂直布局中的材料站
        m_material_panel = new MaterialPanel(this);
        m_monitoring_sizer->Add(m_material_panel, 0, wxALL | wxEXPAND , 0);

        bSizer_status_below->Add(bSizer_left, 1, wxALL | wxEXPAND, 0);

        //第二段水平布局，中间间隔
        auto m_panel_separator_middle = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTAB_TRAVERSAL);
        m_panel_separator_middle->SetBackgroundColour(wxColour(240, 240, 240));
        m_panel_separator_middle->SetMinSize(wxSize(FromDIP(10), -1));

        bSizer_status_below->Add(m_panel_separator_middle, 0, wxEXPAND | wxALL, 0);

        //第二段水平布局，右侧信息
        m_machine_ctrl_panel = new wxPanel(this);
        m_machine_ctrl_panel->SetBackgroundColour(*wxWHITE);
        m_machine_ctrl_panel->SetDoubleBuffered(true);  //开启双缓冲区，避免闪烁
        auto m_machine_control = create_machine_control_page(m_machine_ctrl_panel);
        m_machine_ctrl_panel->SetSizer(m_machine_control);
        m_machine_ctrl_panel->Layout();
        m_machine_control->Fit(m_machine_ctrl_panel);

        bSizer_status_below->Add(m_machine_ctrl_panel, 0, wxALL, 0);

        m_panel_separator_right = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(10), -1), wxTAB_TRAVERSAL);
        m_panel_separator_right->SetBackgroundColour(wxColour(240, 240, 240));

        bSizer_status_below->Add(m_panel_separator_right, 0, wxEXPAND | wxALL, 0);

        bSizer_status->Add(bSizer_status_below, 1, wxALL | wxEXPAND, 0);

        m_panel_separotor_bottom = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(10)), wxTAB_TRAVERSAL);
        m_panel_separotor_bottom->SetBackgroundColour(wxColour(240, 240, 240));

        bSizer_status->Add(m_panel_separotor_bottom, 0, wxEXPAND | wxALL, 0);
        this->SetSizerAndFit(bSizer_status);
        this->Layout();


}

SingleDeviceState::~SingleDeviceState()
{

}

wxBoxSizer* SingleDeviceState::create_top_device_info_page()
{
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        
        //水平布局
        wxBoxSizer *bSizer_title_label = new wxBoxSizer(wxHORIZONTAL);
        m_panel_top_title = new wxPanel(this, wxID_ANY,wxDefaultPosition, wxSize(-1, FromDIP(22)), wxTAB_TRAVERSAL);
        m_panel_top_title->SetBackgroundColour(wxColour(240,240,240));
        //显示设备名称
        m_staticText_device_name = new Label(m_panel_top_title, ("six-AD5M"));
        m_staticText_device_name->Wrap(-1);
        m_staticText_device_name->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_device_name->SetForegroundColour(wxColour(51,51,51));

        bSizer_title_label->Add(m_staticText_device_name, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));
        bSizer_title_label->Add(FromDIP(13), 0, 0, 0);
        bSizer_title_label->AddStretchSpacer();

        //显示设备所在货架
        m_staticText_device_position = new Label(m_panel_top_title, ("shelf2"));
        m_staticText_device_position->Wrap(-1);
        m_staticText_device_position->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_device_position->SetForegroundColour(wxColour(51,51,51));

        bSizer_title_label->Add(m_staticText_device_position, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));
        //bSizer_title_label->Add(FromDIP(13), 0, 0, 0);
        bSizer_title_label->AddStretchSpacer();

        //显示提示内容
        m_staticText_device_tip = new Label(m_panel_top_title, ("error"));
        m_staticText_device_tip->Wrap(-1);
        m_staticText_device_tip->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_device_tip->SetForegroundColour(wxColour(251,71,71));

        bSizer_title_label->Add(m_staticText_device_tip, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));
        //bSizer_title_label->Add(FromDIP(13), 0, 0, 0);
        bSizer_title_label->AddStretchSpacer();

        m_panel_top_title->SetSizer(bSizer_title_label);
        m_panel_top_title->Layout();
        bSizer_title_label->Fit(m_panel_top_title);
        sizer->Add(m_panel_top_title, 0, wxEXPAND | wxALL, 0);

        //添加白色分割条
        auto m_panel_separotor_top = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(3)), wxTAB_TRAVERSAL);
        m_panel_separotor_top->SetBackgroundColour(wxColour(255,255,255));

        sizer->Add(m_panel_separotor_top, 0, wxEXPAND | wxALL, 0);
        return sizer;

}

wxBoxSizer* SingleDeviceState::create_monitoring_page()
{
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        //水平布局
        wxBoxSizer *bSizer_title_label = new wxBoxSizer(wxHORIZONTAL);
        m_panel_top_title = new wxPanel(this, wxID_ANY,wxDefaultPosition, wxSize(-1, FromDIP(22)), wxTAB_TRAVERSAL);
        m_panel_top_title->SetBackgroundColour(wxColour(240,240,240));
        //显示设备名称
        m_staticText_device_name = new Label(m_panel_top_title, ("six-AD5M"));
        m_staticText_device_name->Wrap(-1);
        m_staticText_device_name->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_device_name->SetForegroundColour(wxColour(51,51,51));

        bSizer_title_label->Add(m_staticText_device_name, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));
        bSizer_title_label->Add(FromDIP(13), 0, 0, 0);
        bSizer_title_label->AddStretchSpacer();

        //显示设备所在货架
        m_staticText_device_position = new Label(m_panel_top_title, ("shelf2"));
        m_staticText_device_position->Wrap(-1);
        m_staticText_device_position->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_device_position->SetForegroundColour(wxColour(51,51,51));

        bSizer_title_label->Add(m_staticText_device_position, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));
        //bSizer_title_label->Add(FromDIP(13), 0, 0, 0);
        bSizer_title_label->AddStretchSpacer();

        //显示提示内容
        m_staticText_device_tip = new Label(m_panel_top_title, ("error"));
        m_staticText_device_tip->Wrap(-1);
        m_staticText_device_tip->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_device_tip->SetForegroundColour(wxColour(251,71,71));

        bSizer_title_label->Add(m_staticText_device_tip, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));
        //bSizer_title_label->Add(FromDIP(13), 0, 0, 0);
        bSizer_title_label->AddStretchSpacer();

        m_panel_top_title->SetSizer(bSizer_title_label);
        m_panel_top_title->Layout();
        bSizer_title_label->Fit(m_panel_top_title);

        sizer->Add(m_panel_top_title, 0, wxEXPAND | wxALL, 0);
        
        //添加白色分割条
        auto m_panel_separotor_top = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(3)), wxTAB_TRAVERSAL);
        m_panel_separotor_top->SetBackgroundColour(wxColour(255,255,255));
        sizer->Add(m_panel_separotor_top, 0, wxEXPAND | wxALL, 0);

        //第二段水平布局，相机垂直布局的顶部间隔
        auto m_panel_separotor0 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        m_panel_separotor0->SetBackgroundColour(wxColour(240, 240, 240));
        m_panel_separotor0->SetMinSize(wxSize(-1, FromDIP(10)));
        m_panel_separotor0->SetMaxSize(wxSize(-1, FromDIP(10)));
        sizer->Add(m_panel_separotor0, 0, wxEXPAND, 0);


        //摄像头布局
        m_panel_monitoring_title = new wxPanel(this, wxID_ANY,wxDefaultPosition, wxSize(-1, FromDIP(36)), wxTAB_TRAVERSAL);
        m_panel_monitoring_title->SetBackgroundColour(wxColour(248,248,248));

        //“摄像头”文字布局
        wxBoxSizer *bSizer_monitoring_title;
        bSizer_monitoring_title = new wxBoxSizer(wxHORIZONTAL);

        m_staticText_monitoring = new Label(m_panel_monitoring_title, ("Camera"));
        m_staticText_monitoring->Wrap(-1);
        m_staticText_monitoring->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_monitoring->SetForegroundColour(wxColour(51,51,51));

        bSizer_monitoring_title->Add(m_staticText_monitoring, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, FromDIP(17));
        bSizer_monitoring_title->Add(FromDIP(13), 0, 0, 0);
        bSizer_monitoring_title->AddStretchSpacer();

        //行与行之间的间距使用 wxPanel 进行填充
        sizer->Add(m_panel_monitoring_title, 0, wxEXPAND | wxALL, 0);

        //播放控件
        m_media_ctrl = new wxMediaCtrl2(this);
        m_media_ctrl->SetMinSize(wxSize(FromDIP(574), FromDIP(288)));

        m_media_play_ctrl = new MediaPlayCtrl(this, m_media_ctrl, wxDefaultPosition, wxSize(-1, FromDIP(40)));

        sizer->Add(m_media_ctrl, 1, wxEXPAND | wxALL, 0);
        sizer->Add(m_media_play_ctrl, 0, wxEXPAND | wxALL, 0);
        return sizer;
}

wxBoxSizer* SingleDeviceState::create_machine_control_page(wxWindow *parent)
{
        wxBoxSizer *bSizer_right = new wxBoxSizer(wxVERTICAL);

        //右上角设备提示信息
        wxBoxSizer *bSizer_title_label = new wxBoxSizer(wxHORIZONTAL);
        m_panel_top_right_info = new wxPanel(parent, wxID_ANY,wxDefaultPosition, wxSize(-1, FromDIP(23)), wxTAB_TRAVERSAL);
        m_panel_top_right_info->SetBackgroundColour(wxColour(240,240,240));

        //显示报错信息
        m_staticText_device_info = new Label(m_panel_top_right_info, ("error Info"));
        m_staticText_device_info->Wrap(-1);
        m_staticText_device_info->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_device_info->SetForegroundColour(wxColour(251,71,71));
        m_staticText_device_info->SetBackgroundColour(wxColour(246,203,198));
        m_staticText_device_info->SetMinSize(wxSize(FromDIP(284),FromDIP(23)));
        m_staticText_device_info->SetWindowStyleFlag(wxALIGN_CENTER); 

        bSizer_title_label->Add(m_staticText_device_info, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));
        //bSizer_title_label->Add(FromDIP(13), 0, 0, 0);
        bSizer_title_label->AddStretchSpacer();

        //显示清除按钮
        m_clear_button = new Button(m_panel_top_right_info, wxString("clear"), "", 0, FromDIP(18));
        m_clear_button->SetFont(wxFont(wxFontInfo(16)));
        m_clear_button->SetBorderWidth(1);
        m_clear_button->SetBackgroundColor(wxColour(240,240,240));
        m_clear_button->SetBorderColor(wxColour(50,141,251));
        m_clear_button->SetTextColor(wxColour(50,141,251));
        m_clear_button->SetMinSize((wxSize(FromDIP(34), FromDIP(23))));
        m_clear_button->SetCornerRadius(0);
        bSizer_title_label->Add(m_clear_button, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

        m_panel_top_right_info->SetSizer(bSizer_title_label);
        m_panel_top_right_info->Layout();
        bSizer_title_label->Fit(m_panel_top_right_info);

        bSizer_right->Add(m_panel_top_right_info, 0, wxALL | wxEXPAND, 0);

        //设备信息与信息之间的间隔
        auto m_panel_separotor3 = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        m_panel_separotor3->SetBackgroundColour(wxColour(240, 240, 240));
        m_panel_separotor3->SetMinSize(wxSize(-1, FromDIP(10)));
        m_panel_separotor3->SetMaxSize(wxSize(-1, FromDIP(10)));
        bSizer_right->Add(m_panel_separotor3, 0, wxEXPAND, 0);


        //标题：信息与控制
        m_panel_control_title = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(36)), wxTAB_TRAVERSAL);
        m_panel_control_title->SetBackgroundColour(wxColour(248,248,248));

        wxBoxSizer *bSizer_control_title = new wxBoxSizer(wxHORIZONTAL);
        m_staticText_control             = new Label(m_panel_control_title,("Info and Control"));
        m_staticText_control->Wrap(-1);
        m_staticText_control->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_control->SetForegroundColour(wxColour(51,51,51));

        bSizer_control_title->Add(m_staticText_control, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, FromDIP(17));
        bSizer_control_title->AddStretchSpacer();

        m_panel_control_title->SetSizer(bSizer_control_title);
        m_panel_control_title->Layout();
        bSizer_control_title->Fit(m_panel_control_title);

        //添加标题
        bSizer_right->Add(m_panel_control_title, 0, wxALL | wxEXPAND, 0);
///********
        //先垂直布局（添加空白间距）
        //再水平布局（1、添加左侧空白间距；2、添加中间垂直布局，3、添加右侧垂直布局）
        //最终添加至整体布局

        wxBoxSizer *bSizer_file_info = new wxBoxSizer(wxVERTICAL);
        m_panel_control_info = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(145)), wxTAB_TRAVERSAL);
        m_panel_control_info->SetBackgroundColour(wxColour(255,255,255));

//***顶部白条（分隔）
        auto m_panel_separotor_top = new wxPanel(m_panel_control_info, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(8)), wxTAB_TRAVERSAL);
        m_panel_separotor_top->SetBackgroundColour(wxColour(255,255,255));

        bSizer_file_info->Add(m_panel_separotor_top, 0, wxEXPAND | wxALL, 0);

        //信息部分水平布局
        wxBoxSizer *bSizer_control_info = new wxBoxSizer(wxHORIZONTAL);

//***水平布局添加最左侧空白
        auto m_panel_separotor_left = new wxPanel(m_panel_control_info, wxID_ANY, wxDefaultPosition,wxDefaultSize, wxTAB_TRAVERSAL);
        m_panel_separotor_left->SetBackgroundColour(wxColour(255,255,255));
        m_panel_separotor_left->SetMinSize(wxSize(FromDIP(10), -1));

        bSizer_control_info->Add(m_panel_separotor_left, 0, wxEXPAND | wxALL, 0);

//添加垂直布局（显示文件内容）
        wxBoxSizer *bSizer_control_file_info = new wxBoxSizer(wxVERTICAL);
        auto m_panel_control_file_info = new wxPanel(m_panel_control_info, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(145)), wxTAB_TRAVERSAL);
        m_panel_control_file_info->SetBackgroundColour(wxColour(255,255,255));

        //显示文件名称
        m_staticText_file_name = new Label(m_panel_control_file_info, ("file: 12345678"));
        m_staticText_file_name->Wrap(-1);
        m_staticText_file_name->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_file_name->SetForegroundColour(wxColour(51,51,51));

        bSizer_control_file_info->Add(m_staticText_file_name, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));
        
        //显示设备状态
        m_staticText_device_state = new Label(m_panel_control_file_info, ("pause"));
        m_staticText_device_state->Wrap(-1);
        m_staticText_device_state->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_device_state->SetForegroundColour(wxColour(50,141,251));

        bSizer_control_file_info->Add(m_staticText_device_state, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

//文件中间白条（分隔）
        auto m_panel_separotor_mid = new wxPanel(m_panel_control_file_info, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(11)), wxTAB_TRAVERSAL);
        m_panel_separotor_mid->SetBackgroundColour(wxColour(255,255,255));

        bSizer_control_file_info->Add(m_panel_separotor_mid, 0, wxEXPAND | wxALL, 0);

        //显示倒计时
        m_staticText_count_time = new Label(m_panel_control_file_info, ("00:33:22"));
        m_staticText_count_time->Wrap(-1);
        m_staticText_count_time->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_count_time->SetForegroundColour(wxColour(50,141,251));

        bSizer_control_file_info->Add(m_staticText_count_time, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

        //显示剩余时间标签
        m_staticText_time_label = new Label(m_panel_control_file_info, ("Remaining Time"));
        m_staticText_time_label->Wrap(-1);
        m_staticText_time_label->SetFont(wxFont(wxFontInfo(16)));
        m_staticText_time_label->SetForegroundColour(wxColour(153,153,153));

        bSizer_control_file_info->Add(m_staticText_time_label, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

        //显示进度条
        m_progress_bar = new ProgressBar(m_panel_control_file_info,wxID_ANY,100,wxDefaultPosition,wxSize(FromDIP(101),FromDIP(5)),true);
        bSizer_control_file_info->Add(m_progress_bar, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

        m_panel_control_file_info->SetSizer(bSizer_control_file_info);
        m_panel_control_file_info->Layout();
        bSizer_control_file_info->Fit(m_panel_control_file_info);
        
        bSizer_control_info->Add(m_panel_control_file_info, 0, wxEXPAND | wxALL, 0);

//***水平布局添加中间空白间距
        auto m_panel_separotor_right = new wxPanel(m_panel_control_info, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(58), -1), wxTAB_TRAVERSAL);
        m_panel_separotor_right->SetBackgroundColour(wxColour(255,255,255));

        bSizer_control_info->Add(m_panel_separotor_right, 0, wxEXPAND | wxALL, 0);

//***添加右侧垂直布局
        wxBoxSizer *bSizer_control_material = new wxBoxSizer(wxVERTICAL);
        auto m_panel_control_material = new wxPanel(m_panel_control_info, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(145)), wxTAB_TRAVERSAL);
        m_panel_control_material->SetBackgroundColour(wxColour(255,255,255));

        //***添加右侧材料
        static Slic3r::GUI::BitmapCache cache;
        m_material_weight_pic = create_scaled_bitmap("monitor_item_prediction", this, 16);
        m_material_pic = create_scaled_bitmap("monitor_item_prediction", this, 40);
        
        m_material_weight_staticbitmap = new wxStaticBitmap(m_panel_control_material, wxID_ANY,m_material_weight_pic);
        m_material_weight_staticbitmap->SetMinSize(wxSize(8,8));
        m_material_staticbitmap = new wxStaticBitmap(m_panel_control_material, wxID_ANY,m_material_pic);
        m_material_staticbitmap->SetMinSize(wxSize(47,50));

        m_material_weight_label = new Label(m_panel_control_material,("234g"));
        wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
        hbox->SetMinSize(wxSize(60,-1));
        hbox->Add(m_material_weight_staticbitmap,0, wxRight, FromDIP(3));
        hbox->Add(m_material_weight_label);

        bSizer_control_material->Add(hbox);
        bSizer_control_material->Add(m_material_staticbitmap,0, wxALIGN_CENTER,0);

        m_panel_control_material->SetSizer(bSizer_control_material);
        m_panel_control_material->Layout();
        bSizer_control_material->Fit(m_panel_control_material);
        
        bSizer_control_info->Add(m_panel_control_material);

//***水平布局添加最右侧空白
        auto m_panel_separotor_right2 = new wxPanel(m_panel_control_info, wxID_ANY, wxDefaultPosition,wxDefaultSize, wxTAB_TRAVERSAL);
        m_panel_separotor_right2->SetBackgroundColour(wxColour(255,255,255));
        m_panel_separotor_right2->SetMinSize(wxSize(FromDIP(26), -1));

        bSizer_control_info->Add(m_panel_separotor_right2, 0, wxEXPAND | wxALL, 0);

        bSizer_file_info->Add(bSizer_control_info, 0, wxEXPAND | wxALL, 0);

//**信息与控制布局添加至垂直布局
        m_panel_control_info->SetSizer(bSizer_file_info);
        m_panel_control_info->Layout();
        bSizer_file_info->Fit(m_panel_control_info);

        bSizer_right->Add(m_panel_control_info, 0, wxALL | wxEXPAND, 0);

//****添加暂停打印、取消打印
        //设备信息与打印按钮之间的间隔
        auto m_panel_separotor4 = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        m_panel_separotor4->SetBackgroundColour(wxColour(248,248,248));
        m_panel_separotor4->SetMinSize(wxSize(-1, FromDIP(10)));
        m_panel_separotor4->SetMaxSize(wxSize(-1, FromDIP(10)));
        bSizer_right->Add(m_panel_separotor4, 0, wxEXPAND, 0);


//***打印布局
        wxBoxSizer *bSizer_control_print = new wxBoxSizer(wxHORIZONTAL);
        auto m_panel_control_print = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(30)), wxTAB_TRAVERSAL);
        m_panel_control_print->SetBackgroundColour(wxColour(255,255,255));

        //显示继续打印按钮
        m_print_button = new Button(m_panel_control_print, wxString("pause print"), "device_pause_print", 0, FromDIP(18));
        m_print_button->SetFont(wxFont(wxFontInfo(16)));
        m_print_button->SetBorderWidth(0);
        m_print_button->SetBackgroundColor(wxColour(255,255,255));
        m_print_button->SetBorderColor(wxColour(255,255,255));
        m_print_button->SetTextColor(wxColour(51,51,51));
        m_print_button->SetMinSize((wxSize(FromDIP(158), FromDIP(29))));
        m_print_button->SetCornerRadius(0);
        bSizer_control_print->Add(m_print_button, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

        auto m_panel_separotor_print = new wxPanel(m_panel_control_print, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(6), -1), wxTAB_TRAVERSAL);
        m_panel_separotor_print->SetBackgroundColour(wxColour(240,240,240));

        bSizer_control_print->Add(m_panel_separotor_print, 0, wxEXPAND | wxALL, 0);

        //显示取消打印按钮
        m_cancel_button = new Button(m_panel_control_print, wxString("cancel print"), "device_cancel_print", 0, FromDIP(18));
        m_cancel_button->SetFont(wxFont(wxFontInfo(16)));
        m_cancel_button->SetBorderWidth(0);
        m_cancel_button->SetBackgroundColor(wxColour(255,255,255));
        m_cancel_button->SetBorderColor(wxColour(255,255,255));
        m_cancel_button->SetTextColor(wxColour(51,51,51));
        m_cancel_button->SetMinSize((wxSize(FromDIP(158), FromDIP(29))));
        m_cancel_button->SetCornerRadius(0);
        bSizer_control_print->Add(m_cancel_button, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

//***继续打印布局添加至垂直布局
        m_panel_control_print->SetSizer(bSizer_control_print);
        m_panel_control_print->Layout();
        bSizer_control_print->Fit(m_panel_control_print);

        bSizer_right->Add(m_panel_control_print,0, wxALL | wxEXPAND, 0);

//***添加打印控制和温度布局之间的间隔
        auto m_panel_separotor5 = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        m_panel_separotor5->SetBackgroundColour(wxColour(240,240,240));
        m_panel_separotor5->SetMinSize(wxSize(-1, FromDIP(10)));
        m_panel_separotor5->SetMaxSize(wxSize(-1, FromDIP(10)));
        bSizer_right->Add(m_panel_separotor5, 0, wxEXPAND, 0);

//***温度布局
        wxBoxSizer *bSizer_control_temperature = new wxBoxSizer(wxHORIZONTAL);
        auto m_panel_control_temperature = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(30)), wxTAB_TRAVERSAL);
        m_panel_control_temperature->SetBackgroundColour(wxColour(255,255,255));

        //显示顶部温度控件
        wxWindowID top_id = wxWindow::NewControlId();
        m_tempCtrl_top = new TempInput(m_panel_control_temperature, top_id, wxString("--"), wxString("--"), wxString("device_top_temperature"), wxString("device_top_temperature"), wxDefaultPosition,
                                        wxDefaultSize, wxALIGN_CENTER);

        m_tempCtrl_top->SetMinTemp(20);
        m_tempCtrl_top->SetMaxTemp(120);
        m_tempCtrl_top->SetMinSize((wxSize(FromDIP(106), FromDIP(29))));
        m_tempCtrl_top->SetBorderWidth(0);
        StateColor tempinput_text_colour(std::make_pair(wxColour(51,51,51), (int) StateColor::Disabled), std::make_pair(wxColour(48,58,60), (int) StateColor::Normal));
        m_tempCtrl_top->SetTextColor(tempinput_text_colour);
        StateColor tempinput_border_colour(std::make_pair(*wxWHITE, (int)StateColor::Disabled), std::make_pair(wxColour(0, 150, 136), (int)StateColor::Focused),
                                        std::make_pair(wxColour(0, 150, 136), (int)StateColor::Hovered),std::make_pair(*wxWHITE, (int)StateColor::Normal));
        m_tempCtrl_top->SetBorderColor(tempinput_border_colour);
        bSizer_control_temperature->Add(m_tempCtrl_top, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

        //间距
        auto m_panel_separotor_temp = new wxPanel(m_panel_control_temperature, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(6), -1), wxTAB_TRAVERSAL);
        m_panel_separotor_temp->SetBackgroundColour(wxColour(240,240,240));
        bSizer_control_temperature->Add(m_panel_separotor_temp, 0, wxEXPAND | wxALL, 0);

        //显示底部温度控件
        wxWindowID bottom_id = wxWindow::NewControlId();
        m_tempCtrl_bottom = new TempInput(m_panel_control_temperature, bottom_id, wxString("--"), wxString("--"), wxString("device_bottom_temperature"), wxString("device_bottom_temperature"), wxDefaultPosition,
                                        wxDefaultSize, wxALIGN_CENTER);

        m_tempCtrl_bottom->SetMinTemp(20);
        m_tempCtrl_bottom->SetMaxTemp(120);
        m_tempCtrl_bottom->SetMinSize((wxSize(FromDIP(106), FromDIP(29))));
        m_tempCtrl_bottom->SetBorderWidth(0);
        //StateColor tempinput_text_colour(std::make_pair(wxColour(171, 172, 172), (int) StateColor::Disabled), std::make_pair(wxColour(48,58,60), (int) StateColor::Normal));
        //m_tempCtrl_bottom->SetTextColor(tempinput_text_colour);
        //StateColor tempinput_border_colour(std::make_pair(*wxWHITE, (int)StateColor::Disabled), std::make_pair(wxColour(0, 150, 136), (int)StateColor::Focused),
        //                                std::make_pair(wxColour(0, 150, 136), (int)StateColor::Hovered),std::make_pair(*wxWHITE, (int)StateColor::Normal));
        m_tempCtrl_bottom->SetBorderColor(tempinput_border_colour);
        bSizer_control_temperature->Add(m_tempCtrl_bottom, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

        //间距
        auto m_panel_separotor_temp1 = new wxPanel(m_panel_control_temperature, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(6), -1), wxTAB_TRAVERSAL);
        m_panel_separotor_temp1->SetBackgroundColour(wxColour(240,240,240));
        bSizer_control_temperature->Add(m_panel_separotor_temp1, 0, wxEXPAND | wxALL, 0);

        //显示中间温度控件
        wxWindowID bottom_mid = wxWindow::NewControlId();
        m_tempCtrl_mid = new TempInput(m_panel_control_temperature, bottom_mid, wxString("--"), wxString("--"), wxString("device_mid_temperature"), wxString("device_mid_temperature"), wxDefaultPosition,
                                        wxDefaultSize, wxALIGN_CENTER);

        m_tempCtrl_mid->SetMinTemp(20);
        m_tempCtrl_mid->SetMaxTemp(120);
        m_tempCtrl_mid->SetMinSize((wxSize(FromDIP(106), FromDIP(29))));
        m_tempCtrl_mid->SetBorderWidth(0);
        //StateColor tempinput_text_colour(std::make_pair(wxColour(171, 172, 172), (int) StateColor::Disabled), std::make_pair(wxColour(48,58,60), (int) StateColor::Normal));
        //m_tempCtrl_mid->SetTextColor(tempinput_text_colour);
        //StateColor tempinput_border_colour(std::make_pair(*wxWHITE, (int)StateColor::Disabled), std::make_pair(wxColour(0, 150, 136), (int)StateColor::Focused),
        //                                std::make_pair(wxColour(0, 150, 136), (int)StateColor::Hovered),std::make_pair(*wxWHITE, (int)StateColor::Normal));
        m_tempCtrl_mid->SetBorderColor(tempinput_border_colour);
        bSizer_control_temperature->Add(m_tempCtrl_mid, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

//***温度布局添加至垂直布局
        m_panel_control_temperature->SetSizer(bSizer_control_temperature);
        m_panel_control_temperature->Layout();
        bSizer_control_temperature->Fit(m_panel_control_temperature);

        bSizer_right->Add(m_panel_control_temperature,0, wxALL | wxEXPAND, 0);

//***添加温度布局和灯布局之间的间隔
        auto m_panel_separotor6 = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        m_panel_separotor6->SetBackgroundColour(wxColour(240,240,240));
        m_panel_separotor6->SetMinSize(wxSize(-1, FromDIP(10)));
        m_panel_separotor6->SetMaxSize(wxSize(-1, FromDIP(10)));
        bSizer_right->Add(m_panel_separotor6, 0, wxEXPAND, 0);

//***灯控制布局
        wxBoxSizer *bSizer_control_lamp = new wxBoxSizer(wxHORIZONTAL);
        auto m_panel_control_lamp = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FromDIP(30)), wxTAB_TRAVERSAL);
        m_panel_control_lamp->SetBackgroundColour(wxColour(255,255,255));

        //显示文件信息按钮
        m_device_info_button = new Button(m_panel_control_lamp, wxString(""), "device_file_info", 0, FromDIP(18));
        m_device_info_button->SetFont(wxFont(wxFontInfo(16)));
        m_device_info_button->SetBorderWidth(0);
        m_device_info_button->SetBackgroundColor(wxColour(255,255,255));
        m_device_info_button->SetBorderColor(wxColour(255,255,255));
        //m_device_info_button->SetTextColor(wxColour(51,51,51));
        m_device_info_button->SetMinSize((wxSize(FromDIP(108), FromDIP(29))));
        m_device_info_button->SetCornerRadius(0);
        bSizer_control_lamp->Add(m_device_info_button, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

        //显示灯控制按钮
        m_lamp_control_button = new Button(m_panel_control_lamp, wxString(""), "device_lamp_control", 0, FromDIP(18));
        m_lamp_control_button->SetFont(wxFont(wxFontInfo(16)));
        m_lamp_control_button->SetBorderWidth(0);
        m_lamp_control_button->SetBackgroundColor(wxColour(255,255,255));
        m_lamp_control_button->SetBorderColor(wxColour(255,255,255));
        //m_lamp_control_button->SetTextColor(wxColour(51,51,51));
        m_lamp_control_button->SetMinSize((wxSize(FromDIP(108), FromDIP(29))));
        m_lamp_control_button->SetCornerRadius(0);
        bSizer_control_lamp->Add(m_lamp_control_button, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

        //显示过滤按钮
        m_filter_button = new Button(m_panel_control_lamp, wxString(""), "device_filter", 0, FromDIP(18));
        m_filter_button->SetFont(wxFont(wxFontInfo(16)));
        m_filter_button->SetBorderWidth(0);
        m_filter_button->SetBackgroundColor(wxColour(255,255,255));
        m_filter_button->SetBorderColor(wxColour(255,255,255));
        //m_filter_button->SetTextColor(wxColour(51,51,51));
        m_filter_button->SetMinSize((wxSize(FromDIP(108), FromDIP(29))));
        m_filter_button->SetCornerRadius(0);
        bSizer_control_lamp->Add(m_filter_button, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(4));

//***灯控制布局添加至垂直布局
        m_panel_control_lamp->SetSizer(bSizer_control_lamp);
        m_panel_control_lamp->Layout();
        bSizer_control_lamp->Fit(m_panel_control_lamp);

        bSizer_right->Add(m_panel_control_lamp,0, wxALL | wxEXPAND, 0);

//***

//

        return bSizer_right;

}

void SingleDeviceState::msw_rescale()
{
	int test = 9;
}

}
}