#ifndef slic3r_DeviceListPanel_hpp_
#define slic3r_DeviceListPanel_hpp_

#include "slic3r/GUI/Widgets/Button.hpp"
#include "slic3r/GUI/Widgets/ComboBox.hpp"
#include "slic3r/GUI/Widgets/PopupWindow.hpp"

namespace Slic3r {
namespace GUI {

#define LISTBOX_HEIGHT 40

class CustomComboBox : public wxPanel
{
private:
    wxStaticText    *m_staticText_name;
    wxStaticBitmap  *m_staticBitmap;
    wxString         m_name;

public:
    CustomComboBox(wxWindow* parent = nullptr, const wxString& name = wxEmptyString);

private:
    void initControl();
    void setCustomBoxSizer();
};

class ListBoxPopup : public PopupWindow
{
public:
    ListBoxPopup(wxWindow *parent, const wxArrayString& names);

    // PopupWindow virtual methods are all overridden to log them
    virtual void Popup(wxWindow *focus = NULL) wxOVERRIDE;
    virtual void OnDismiss() wxOVERRIDE;
    virtual bool ProcessLeftDown(wxMouseEvent &event) wxOVERRIDE;
    virtual bool Show(bool show = true) wxOVERRIDE;

    bool was_dismiss() { return m_dismiss; }

    void resetSize(const wxSize &size);

private:
    bool         m_dismiss { false };
    wxListBox   *m_listBox;
};

class DeviceListPanel : public wxPanel
{
private:
    CustomComboBox* m_comboBox_position;
    ListBoxPopup*   m_listBox_position;
    CustomComboBox *m_comboBox_status;
    ListBoxPopup   *m_listBox_status;
    CustomComboBox *m_comboBox_type;
    wxButton*       m_btn_outer_net;
    wxButton*       m_btn_inner_net;
    wxButton*       m_btn_mode;
    wxStaticBitmap* m_bitmap_no_device;
    wxStaticText*   m_staticText_no_device;

public:
    DeviceListPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, 
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
    ~DeviceListPanel();

    void msw_rescale();

private:
    void initControl();
    void initAllDeviceStatus(wxArrayString& names);
    void setPanelBoxSizer();
    void connectEvent();

    void on_comboBox_position_clicked(wxMouseEvent &event);
    void on_comboBox_status_clicked(wxMouseEvent &event);
};

} // GUI
} // Slic3r

#endif /* slic3r_DeviceListPanel_hpp_ */
