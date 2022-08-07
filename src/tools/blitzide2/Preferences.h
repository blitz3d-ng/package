#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

struct Preferences{
	wxString signId;
	wxString teamId;

	void ResetToDefaults();

	void Load( const wxString &blitzpath );
	void Save( const wxString &blitzpath );
};

#endif
