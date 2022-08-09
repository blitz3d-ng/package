#ifndef PREFERENCES_DIALOG
#define PREFERENCES_DIALOG

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "Preferences.h"

class PreferencesDialog:public wxDialog
{
public:
	PreferencesDialog( wxWindow *parent,Preferences *prefs );
private:
	wxNotebook *nb;
	Preferences *prefs;

	struct SigningIdentity{
		wxString label,id;

		SigningIdentity( wxString label,wxString id ):label(label),id(id){}
	};

	struct ProvisioningProfile{
		wxString id;

		ProvisioningProfile( wxString id ):id(id){}
	};

	std::vector<SigningIdentity> signers;
	std::vector<ProvisioningProfile> teams;

	void OnSignIdChange( wxCommandEvent& event );
	void OnTeamIdChange( wxCommandEvent& event );

	DECLARE_EVENT_TABLE()
};


#endif
