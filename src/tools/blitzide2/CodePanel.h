#ifndef CODE_PANEL_H
#define CODE_PANEL_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/listbox.h>

WX_DECLARE_HASH_MAP( int,int,wxIntegerHash,wxIntegerEqual,LineLookup );

class CodePanel:public wxPanel{
private:
	wxNotebook *nb;

	wxListBox *functionList;
	LineLookup functions;

	wxListBox *typeList;
	LineLookup types;

	wxListBox *labelList;
	LineLookup labels;

public:
	CodePanel( wxWindow *parent );

	void updateLists( wxString source );
};

#endif
