#ifndef _Slic3r_GUI_FFButton_hpp_
#define _Slic3r_GUI_FFButton_hpp_
#include <wx/tglbtn.h>


class FFButton : public wxToggleButton
{
public:
	FFButton(wxWindow* parent, const wxString& label = "", wxWindowID id = wxID_ANY);
	~FFButton() {};

	void SetValue(bool state) override;

private:
	void updateState();

private:
	bool		m_hoverFlag;
	bool		m_pressFlag;
	wxColour	m_normalColor;
	wxColour	m_normalHoverColor;
	wxColour	m_normalPressColor;
	wxColour	m_selectColor;
	wxColour	m_selectHoverColor;
	wxColour	m_selectPressColor;
};


#endif /* _Slic3r_GUI_FFButton_hpp_ */