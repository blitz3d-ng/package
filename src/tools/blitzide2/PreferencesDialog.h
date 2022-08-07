#ifndef PREFERENCES_DIALOG
#define PREFERENCES_DIALOG

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "Preferences.h"

class PreferencesDialog:public wxDialog
{
public:
	PreferencesDialog( wxWindow *parent,Preferences *prefs );
private:
	wxNotebook *nb;
	Preferences *prefs;

	DECLARE_EVENT_TABLE()
};


#endif
