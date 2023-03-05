#ifndef BUILD_DIALOG
#define BUILD_DIALOG

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class BuildDialog:public wxDialog
{
public:
	BuildDialog( wxWindow * parent );

	void AddMessage( const wxString &m );
private:
	wxTextCtrl *output;

	void OnTerminate( wxCommandEvent & event );

	DECLARE_EVENT_TABLE()
};


#endif
