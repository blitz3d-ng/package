#ifndef FILEVIEW_H
#define FILEVIEW_H

#include "std.h"

class FileView : public wxPanel{
private:
  static void LoadKeywords();
private:
  wxString path, source;
public:
  FileView( wxString &path,wxWindow *parent,wxWindowID id );

  bool Save();
  bool Save( wxString &newPath );
};

#endif
