#ifndef PREFERENCES_DIALOG
#define PREFERENCES_DIALOG

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class PreferencesDialog:public wxDialog
{
public:
	PreferencesDialog( wxWindow * parent );
private:
	wxNotebook *nb;

	DECLARE_EVENT_TABLE()
};


#endif
