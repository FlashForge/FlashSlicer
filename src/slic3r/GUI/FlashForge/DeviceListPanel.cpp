#include "DeviceListPanel.hpp"
#include "slic3r/GUI/I18N.hpp"

namespace Slic3r {
namespace GUI {

CustomComboBox::CustomComboBox(wxWindow* parent, const wxString& name)
    : wxPanel(parent)
    , m_name(name)
{
    initControl();
    setCustomBoxSizer();
}

void CustomComboBox::initControl()
{
    m_staticText_name = new wxStaticText(this, wxID_ANY, m_name);
    m_staticBitmap = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
    m_staticBitmap->SetBitmap(create_scaled_bitmap("custom_comboBox_down", nullptr, 10));
}

void CustomComboBox::setCustomBoxSizer()
{
    wxBoxSizer *hTopBoxSizer = new wxBoxSizer(wxLI_HORIZONTAL);
    hTopBoxSizer->Add(m_staticText_name, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    hTopBoxSizer->AddSpacer(20);
    hTopBoxSizer->Add(m_staticBitmap, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    SetSizer(hTopBoxSizer);
    Layout();
    Fit();
}

ListBoxPopup::ListBoxPopup(wxWindow *parent, const wxArrayString &names)
    : PopupWindow(parent, wxBORDER_NONE | wxPU_CONTAINS_CONTROLS), m_dismiss(false)
{
    m_listBox = new wxListBox(this, wxID_ANY);
    m_listBox->InsertItems(names, m_listBox->GetCount());
    m_listBox->SetSelection(0);

    wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);
    vSizer->Add(m_listBox, 0, wxALL);
    SetSizer(vSizer);
    Layout();
    Fit();
}

void ListBoxPopup::Popup(wxWindow *WXUNUSED(focus))
{
    PopupWindow::Popup();
}

void ListBoxPopup::OnDismiss()
{

}

bool ListBoxPopup::ProcessLeftDown(wxMouseEvent &event)
{
    return PopupWindow::ProcessLeftDown(event);
}

bool ListBoxPopup::Show(bool show)
{
    return PopupWindow::Show(show);
}

void ListBoxPopup::resetSize(const wxSize &size)
{
    m_listBox->SetSize(size.x, m_listBox->GetCount() * LISTBOX_HEIGHT);
    SetSize(size.x, GetSize().y);
    Fit();
}




BEGIN_EVENT_TABLE(MachineItemPanel, wxPanel)

EVT_LEFT_DOWN(MachineItemPanel::mouseDown)
EVT_LEFT_UP(MachineItemPanel::mouseReleased)
EVT_MOUSE_CAPTURE_LOST(MachineItemPanel::mouseCaptureLost)
EVT_ENTER_WINDOW(MachineItemPanel::onEnterPanel)
EVT_LEAVE_WINDOW(MachineItemPanel::onLeavePanel)

END_EVENT_TABLE()

MachineItemPanel::MachineItemPanel(wxWindow *parent, int connectId /* = -1*/) 
    : wxPanel(parent)
    , m_connectId(connectId) 
{ 
    initControl();
    setPanelBoxSizer();
}

void MachineItemPanel::initControl()
{
    m_staticText_name = new wxStaticText(this, wxID_ANY, wxT("AD5M"));
    m_staticBitmap_Icon = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
    m_staticBitmap_Icon->SetBitmap(create_scaled_bitmap("device_Ad5m", nullptr, 100));
    m_staticText_position = new wxStaticText(this, wxID_ANY, wxT("Store1"));
    m_staticText_status   = new wxStaticText(this, wxID_ANY, wxT("Idle"));

    SetBackgroundColour(wxColour("#FFFFFF"));
}

void MachineItemPanel::setPanelBoxSizer() 
{
    wxBoxSizer *hIconSizer = new wxBoxSizer(wxHORIZONTAL);
    hIconSizer->AddStretchSpacer();
    hIconSizer->Add(m_staticBitmap_Icon);
    hIconSizer->AddStretchSpacer();

    wxBoxSizer *vBoxSizer = new wxBoxSizer(wxVERTICAL);
    vBoxSizer->Add(m_staticText_name, 0, wxALL, 10);
    vBoxSizer->Add(hIconSizer, 1, wxEXPAND);
    vBoxSizer->Add(m_staticText_position, 0, wxALL, 10);
    vBoxSizer->Add(m_staticText_status, 0, wxALL, 10);

    SetSizer(vBoxSizer);
    m_staticBitmap_Icon->SetSize(176, 99);
    SetMinSize(wxSize(250, 250));
    Layout();
    Fit();
}

void MachineItemPanel::mouseDown(wxMouseEvent& event)
{
    event.Skip();
    m_pressed = true;
    if (m_canFocus)
        SetFocus();
    if (!HasCapture())
        CaptureMouse();  // get mouse input, it's useful in cur panel.
}
void MachineItemPanel::mouseReleased(wxMouseEvent& event)
{
    event.Skip();
    if (m_pressed) {
        m_pressed = false;
        if (HasCapture())
            ReleaseMouse();  // release last mouse capture
        if (wxRect({0, 0}, GetSize()).Contains(event.GetPosition()))
            sendButtonEvent();
    }
}
void MachineItemPanel::mouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    wxMouseEvent evt;
    mouseReleased(evt);
}

void MachineItemPanel::onEnterPanel(wxMouseEvent& event)
{
    if (!m_hovered)
        m_hovered = true;
}

void MachineItemPanel::onLeavePanel(wxMouseEvent& event)
{
    if (m_hovered) {
        m_hovered = false;
        m_pressed = false;
    }
}

void MachineItemPanel::sendButtonEvent()
{
    wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
    event.SetEventObject(this);
    GetEventHandler()->ProcessEvent(event);
}

DevicePanel::DevicePanel(wxWindow *parent) 
    : wxPanel(parent)
{
    MachineItemPanel *item1 = new MachineItemPanel(this);
    MachineItemPanel *item2 = new MachineItemPanel(this);
    wxGridSizer      *gridSizer = new wxGridSizer(5, FromDIP(5), FromDIP(5));
    gridSizer->Add(item1, 1, wxALL, 20);
    gridSizer->Add(item2, 1, wxALL, 20);

    SetSizer(gridSizer);
    Layout();
    Fit();

    SetBackgroundColour(wxColour(89, 129, 178, 0.5));
}




DeviceListPanel::DeviceListPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, 
        const wxSize& size, long style, const wxString& name)
    : wxPanel(parent, id, pos, size, style, name)
{
    this->SetBackgroundColour(0xEEEEEE);

    initControl();
    setPanelBoxSizer();
    connectEvent();
}

DeviceListPanel::~DeviceListPanel()
{
    
}

void DeviceListPanel::msw_rescale()
{
	
}

void DeviceListPanel::initControl()
{
    m_comboBox_position = new CustomComboBox(this, _L("Position"));
    wxArrayString names;
    names.Add("All");
    names.Add("Offen");
    names.Add("Store1");
    names.Add("Store2");
    m_listBox_position = new ListBoxPopup(this, names);
    m_comboBox_status  = new CustomComboBox(this, _L("Status"));
    initAllDeviceStatus(names);
    m_listBox_status = new ListBoxPopup(this, names);
    m_comboBox_type = new CustomComboBox(this, _L("Machine Type"));
    m_btn_outer_net = new wxButton(this, wxID_ANY, _L("Outer Net"));
    m_btn_inner_net = new wxButton(this, wxID_ANY, _L("LAN"));
    m_btn_mode         = new wxButton(this, wxID_ANY, _L("Mode"));
    m_bitmap_no_device = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
    m_bitmap_no_device->SetBitmap(create_scaled_bitmap("monitor_device_empty", nullptr, 250));
    m_staticText_no_device = new wxStaticText(this, wxID_ANY, wxT("No Device"));
    m_staticText_no_device->Wrap(-1);
    m_staticText_no_device->SetForegroundColour(0x909090);
    m_machinePanel = new DevicePanel(this);
    m_machinePanel->Show(false);
}

void DeviceListPanel::initAllDeviceStatus(wxArrayString &names)
{
    names.Clear();
    names.Add("All");
    names.Add("Idle");
    names.Add("Busy");
    names.Add("Error");
    names.Add("Printing");
}

void DeviceListPanel::setPanelBoxSizer()
{
    wxBoxSizer* hTopSizer = new wxBoxSizer(wxHORIZONTAL);
    hTopSizer->AddSpacer(20);
    hTopSizer->Add(m_comboBox_position, 0, wxALL | wxALIGN_CENTER_VERTICAL);
    hTopSizer->AddSpacer(50);
    hTopSizer->Add(m_comboBox_status, 0, wxALL | wxALIGN_CENTER_VERTICAL);
    hTopSizer->AddSpacer(50);
    hTopSizer->Add(m_comboBox_type, 0, wxALL | wxALIGN_CENTER_VERTICAL);
    hTopSizer->AddSpacer(800);
    hTopSizer->Add(m_btn_outer_net, 0, wxALL | wxALIGN_CENTER_VERTICAL);
    hTopSizer->AddSpacer(10);
    hTopSizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(1, 28), wxLI_VERTICAL), 0, wxALIGN_CENTER_VERTICAL, 10);
    hTopSizer->AddSpacer(10);
    hTopSizer->Add(m_btn_inner_net, 0, wxALL | wxALIGN_CENTER_VERTICAL);
    hTopSizer->AddSpacer(30);
    hTopSizer->Add(m_btn_mode, 0, wxALL | wxALIGN_RIGHT);
    hTopSizer->AddSpacer(20);

    m_noDevice_sizer = new wxBoxSizer(wxVERTICAL);
    m_noDevice_sizer->AddStretchSpacer();
    m_noDevice_sizer->Add(m_bitmap_no_device, 0, wxALIGN_CENTER_HORIZONTAL);
    m_noDevice_sizer->AddSpacer(20);
    m_noDevice_sizer->Add(m_staticText_no_device, 1, wxALL | wxALIGN_CENTER_HORIZONTAL);
    m_noDevice_sizer->AddStretchSpacer();

    wxStaticLine *horLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(160, 1), wxLI_HORIZONTAL);
    horLine->SetBackgroundColour(wxColour(255, 255, 255, 0));
    wxBoxSizer *vAllSizer = new wxBoxSizer(wxVERTICAL);
    vAllSizer->Add(hTopSizer);
    vAllSizer->Add(horLine, 0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND, 10);
    vAllSizer->Add(m_noDevice_sizer, 1, wxALL | wxALIGN_CENTER_HORIZONTAL);
    vAllSizer->Add(m_machinePanel, 1, wxEXPAND);

    this->SetSizer(vAllSizer);
    this->Layout();
    this->Fit();
}

void DeviceListPanel::connectEvent()
{
    m_comboBox_position->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &e) {
        on_comboBox_position_clicked(e);
        });
    m_comboBox_status->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &e) { 
        on_comboBox_status_clicked(e); 
        });

    m_btn_mode->Bind(wxEVT_BUTTON, &DeviceListPanel::onModelBtnClicked, this);
}

void DeviceListPanel::on_comboBox_position_clicked(wxMouseEvent &event)
{
    auto    mouse_pos = ClientToScreen(event.GetPosition());
    wxPoint rect      = m_comboBox_position->ClientToScreen(wxPoint(0, 0));
    wxPoint pos       = m_comboBox_position->ClientToScreen(wxPoint(0, 0));
    pos.y += m_comboBox_position->GetRect().height;
    m_listBox_position->Move(pos);
    m_listBox_position->resetSize(wxSize(m_comboBox_position->GetSize().x, -1));
    m_listBox_position->Popup();
}

void DeviceListPanel::on_comboBox_status_clicked(wxMouseEvent &event) 
{
    auto    mouse_pos = ClientToScreen(event.GetPosition());
    wxPoint rect      = m_comboBox_status->ClientToScreen(wxPoint(0, 0));
    wxPoint pos       = m_comboBox_status->ClientToScreen(wxPoint(0, 0));
    pos.y += m_comboBox_status->GetRect().height;
    m_listBox_status->Move(pos);
    m_listBox_status->resetSize(wxSize(m_comboBox_status->GetSize().x, -1));
    m_listBox_status->Popup();
}

void DeviceListPanel::onModelBtnClicked(wxCommandEvent &event)
{
    m_noDevice_sizer->Show(false);
    m_machinePanel->Show(true);
    Layout();
}

} // GUI
} // Slic3r