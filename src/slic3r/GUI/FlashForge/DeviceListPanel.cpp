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
    hTopSizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(1, 28), wxLI_VERTICAL), 0, wxALL);
    hTopSizer->AddSpacer(10);
    hTopSizer->Add(m_btn_inner_net, 0, wxALL | wxALIGN_CENTER_VERTICAL);
    hTopSizer->AddSpacer(30);
    hTopSizer->Add(m_btn_mode, 0, wxALL | wxALIGN_RIGHT);
    hTopSizer->AddSpacer(20);

    wxStaticLine *horLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(1600, 1), wxLI_HORIZONTAL);
    horLine->SetBackgroundColour(wxColour(255, 255, 255, 0));
    wxBoxSizer *vAllSizer = new wxBoxSizer(wxVERTICAL);
    vAllSizer->Add(hTopSizer);
    vAllSizer->Add(horLine, 0, wxALIGN_CENTER_HORIZONTAL);
    vAllSizer->AddStretchSpacer();
    vAllSizer->Add(m_bitmap_no_device, 0, wxALIGN_CENTER_HORIZONTAL);
    vAllSizer->AddSpacer(20);
    vAllSizer->Add(m_staticText_no_device, 1, wxALL | wxALIGN_CENTER_HORIZONTAL);
    vAllSizer->AddStretchSpacer();

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

} // GUI
} // Slic3r