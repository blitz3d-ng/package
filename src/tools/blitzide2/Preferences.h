#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

struct Preferences{
	wxString path;

	wxString signId,teamId;

	void ResetToDefaults();

	void Load( const wxString &blitzpath );
	void Save();
};

#endif
