#ifndef FILEVIEW_H
#define FILEVIEW_H

#include "std.h"
#include <wx/stc/stc.h>
#include "BlitzCC.h"

class FileView : public wxPanel{
private:
  static void LoadKeywords();

  wxString path, source;
	wxStyledTextCtrl* text;
	bool dirty;
	BlitzCC *cc;

	void Open( wxString &path );

	void OnTextEvent( wxStyledTextEvent& event );
public:
  FileView( wxString &path,wxWindow *parent,wxWindowID id );

	wxString GetTitle();

  bool Save();
  bool Save( wxString &newPath );

	void Execute( const Target &target,const Preferences *prefs );
	void Kill();
	void Build( wxString &out );
};

#endif
