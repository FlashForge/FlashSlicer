#include "ImageButton.hpp"

namespace Slic3r::GUI {

ImageButton::ImageButton(wxWindow* parent, const wxString& normal, const wxString& hover,
	const wxString& press, const wxString& disable/*= ""*/)
	: wxBitmapButton(parent, wxID_ANY, wxBitmap())
{
	if (!normal.IsEmpty()) {
		m_normal = create_scaled_bitmap(normal.ToStdString(), this);
	}
	if (!hover.IsEmpty()) {
		m_hover = create_scaled_bitmap(hover.ToStdString(), this);
	} else {
		m_hover = m_normal;
	}
	if (!press.IsEmpty()) {
		m_press = create_scaled_bitmap(press.ToStdString(), this);
	} else {
		m_press = m_hover;
	}	
	if (!disable.IsEmpty()) { 
		m_disable = create_scaled_bitmap(press.ToStdString(), this);
	} else {
		m_disable = m_normal;
	}
	SetBitmap(m_normal);
	
	Bind(wxEVT_ENTER_WINDOW, &ImageButton::OnMouseEnter, this);
	Bind(wxEVT_LEAVE_WINDOW, &ImageButton::OnMouseLeave, this);
	Bind(wxEVT_LEFT_DOWN, &ImageButton::OnMouseDown, this);
	Bind(wxEVT_LEFT_UP, &ImageButton::OnMouseUp, this);
}

void ImageButton::OnMouseEnter(wxMouseEvent& event)
{
	SetBitmap(m_hover);
	Update();
}

void ImageButton::OnMouseLeave(wxMouseEvent& event)
{
	SetBitmap(m_normal);
	Update();
}

void ImageButton::OnMouseDown(wxMouseEvent& event)
{
	SetBitmap(m_press);
	Update();
}

void ImageButton::OnMouseUp(wxMouseEvent& event)
{
	SetBitmap(m_normal);
	Update();
}

} // end namespace 