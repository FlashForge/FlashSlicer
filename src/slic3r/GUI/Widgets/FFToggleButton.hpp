#ifndef _Slic3r_GUI_FFToggleButton_hpp_
#define _Slic3r_GUI_FFToggleButton_hpp_
#include <wx/tglbtn.h>


class FFToggleButton : public wxToggleButton
{
public:
	FFToggleButton(wxWindow* parent, const wxString& label = "", wxWindowID id = wxID_ANY);
	~FFToggleButton() {};

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

#endif /* _Slic3r_GUI_FFToggleButton_hpp_ */