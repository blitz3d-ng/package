#ifndef FILEVIEW_H
#define FILEVIEW_H

#include "std.h"
#include <wx/stc/stc.h>

class FileView : public wxPanel{
private:
  static void LoadKeywords();
private:
  wxString path, source;
public:
  FileView( wxString &path,wxWindow *parent,wxWindowID id );

  bool Save();
  bool Save( wxString &newPath );

	void Execute();
	void Build( wxString &out );
};

#endif
