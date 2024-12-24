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

	void EmitDirtyEvent();
public:
  FileView( wxString &path,wxWindow *parent,wxWindowID id );

	wxString GetTitle();
	wxString GetPath();
	bool IsDirty();

  bool Save();
  bool Save( wxString &newPath );

	void Execute( const Target &target,const Preferences *prefs );
	void Kill();
	void Build( wxString &out );
};

wxDECLARE_EVENT(FILE_VIEW_DIRTY_EVENT, wxCommandEvent);

#endif
