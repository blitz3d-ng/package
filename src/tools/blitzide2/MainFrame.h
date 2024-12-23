#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/notebook.h>
#include <wx/filedlg.h>
#include <wx/filename.h>

#include "HtmlHelp.h"
#include "FileView.h"
#include "BuildDialog.h"
#include "Preferences.h"

class MainFrame:public wxFrame{
public:
	MainFrame( const wxString& title );

	void AddFile( wxString &path );
	void AddFiles( wxArrayString &path );

	void UpdateMenu( int pageIndex );
	void UpdateToolbar( int pageIndex );
private:
	wxMenuBar *menuBar;
	wxToolBar *toolbar;
	HtmlHelp *help;
	wxNotebook *nb;
	FileView *buildFile;
	wxMenu *platformMenu;

	Preferences prefs;
	BuildDialog *buildDialog;
	std::vector<Target> devices;
	int deviceIdx;

	void UnlockFile();
	void EnumerateDevices();

	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

	void OnNew( wxCommandEvent& event );
	void OnOpen( wxCommandEvent& event );
	void OnClose( wxCommandEvent& event );
	void OnSave( wxCommandEvent& event );
	void OnPreferences( wxCommandEvent& event );
	void OnRun( wxCommandEvent& event );
	void OnExe( wxCommandEvent& event );
	void OnTarget( wxCommandEvent& event );
	void OnAddFile( wxCommandEvent& event );
	void OnBuildBegin( wxCommandEvent& event );
	void OnBuildProgress( wxCommandEvent& event );
	void OnBuildEnd( wxCommandEvent& event );
	void OnBuildKill( wxCommandEvent& event );
	void OnLockFile( wxCommandEvent& event );
	void OnUnlockFile( wxCommandEvent& event );
	void OnHome( wxCommandEvent& event );
	void OnBack( wxCommandEvent& event );
	void OnForward( wxCommandEvent& event );

	void OnPageChanged( wxBookCtrlEvent& event );

	wxDECLARE_EVENT_TABLE();
};

#endif
