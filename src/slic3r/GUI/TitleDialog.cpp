#include "TitleDialog.hpp"
#include <wx/stattext.h>


namespace Slic3r::GUI {

TitleBar::TitleBar(wxWindow *parent, const wxString& title, const wxColour& color, int borderRadius/*= 6*/)
    : wxWindow(parent, wxID_ANY)
    , m_dragging(false)
    , m_bgColor(color)
    , m_title(title)
{
    SetBackgroundColour(color);

    m_titleLbl = new wxStaticText(this, wxID_ANY, m_title);
    m_closeBtn = new ImageButton(this, "title_close", "title_closeHover", "title_closePress");

    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(m_titleLbl, wxEXPAND | wxALIGN_CENTER);
    mainSizer->AddStretchSpacer(1);
    mainSizer->Add(m_closeBtn, 0, wxALL);
    SetSizer(mainSizer);
    
    //Bind(wxEVT_PAINT, &TitleBar::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &TitleBar::OnMouseLeftDown, this);
    //Bind(wxEVT_LEFT_UP, &TitleBar::OnMouseLeftUp, this);
    //Bind(wxEVT_MOTION, &TitleBar::OnMouseMotion, this);
    Bind(wxEVT_MOUSE_CAPTURE_LOST, &TitleBar::OnMouseCaptureLost, this);
    //m_closeBtn->Connect(wxEVT_BUTTON, &TitleBar::OnClose, this);
    Bind(wxEVT_BUTTON, &TitleBar::OnClose, this);
}

wxSize TitleBar::DoGetBestClientSize() const
{
    return wxSize(-1, 48);
}

//void TitleBar::SetBackgroundColor(const wxColour& color)
//{
//    SetBackgroundColor
//}

void TitleBar::SetTitle(const wxString& title)
{
    m_titleLbl->SetLabel(title);
}

void TitleBar::OnPaint(wxPaintEvent& event)
{
    
}

void TitleBar::OnMouseLeftDown(wxMouseEvent &event)
{
    if (!m_dragging) {
        Bind(wxEVT_LEFT_UP, &TitleBar::OnMouseLeftUp, this);
        Bind(wxEVT_MOTION, &TitleBar::OnMouseMotion, this);
        m_dragging = true;

        wxPoint clientStart = event.GetPosition();
        m_dragStartMouse = ClientToScreen(clientStart);
        m_dragStartWindow = GetParent()->GetPosition();
        CaptureMouse();
    }
}
void TitleBar::OnMouseLeftUp(wxMouseEvent &event)
{
    FinishDrag();
}

void TitleBar::OnMouseMotion(wxMouseEvent &event)
{
    wxPoint curClientPnt = event.GetPosition();
    wxPoint curScreenPnt = ClientToScreen(curClientPnt);
    wxPoint movementVector = curScreenPnt - m_dragStartMouse;
    GetParent()->SetPosition(m_dragStartWindow + movementVector);
}

void TitleBar::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    FinishDrag();
}

void TitleBar::OnClose(wxCommandEvent &event)
{
    if (GetParent()) {
        GetParent()->Close();
    }
}

void TitleBar::FinishDrag()
{
    if (m_dragging) {
        Unbind(wxEVT_LEFT_UP, &TitleBar::OnMouseLeftUp, this);
        Unbind(wxEVT_MOTION, &TitleBar::OnMouseMotion, this);
        m_dragging = false;
    }
    if (HasCapture()) {
        ReleaseMouse();
    }
}


// TitleDialog
TitleDialog::TitleDialog(wxWindow* parent, const wxString& title, int borderRadius/*=6*/)
    : DPIDialog(parent, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxTRANSPARENT_WINDOW)
    , m_titleBar(new TitleBar(this, title, "#E1E2E6", borderRadius))
    , m_mainSizer(new wxBoxSizer(wxVERTICAL))
{
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_titleBar, wxSizerFlags(0).Expand());
    sizer->Add(m_mainSizer, wxSizerFlags(0).Expand());
    SetSizer(sizer);
    Layout();
}

void TitleDialog::SetTitleBackgroundColor(const wxColour& color)
{
    m_titleBar->SetBackgroundColour(color);
}

wxBoxSizer* TitleDialog::MainSizer()
{
    return m_mainSizer;
}

} // end namespace