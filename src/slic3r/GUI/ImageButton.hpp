#ifndef slic3r_GUI_ImageButton_hpp_
#define slic3r_GUI_ImageButton_hpp_

#include <wx/bmpbuttn.h>
#include "wxExtensions.hpp"

namespace Slic3r::GUI {

class ImageButton : public wxBitmapButton
{
public:
	ImageButton(wxWindow* parent, const wxString& normal, const wxString& hover,
		const wxString& press, const wxString& disable = "");
	
protected:
	//void OnPaint(wxPaintEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseUp(wxMouseEvent& event);

private:
	wxBitmap	m_normal;
	wxBitmap	m_hover;
	wxBitmap	m_press;
	wxBitmap	m_disable;
};

} // end namespace

#endif /* slic3r_GUI_ImageButton_hpp_ */