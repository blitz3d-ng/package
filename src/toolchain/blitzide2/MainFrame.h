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

class MainFrame: public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void AddFile( wxString &path );
    void AddFiles( wxArrayString &path );

    void UpdateToolbar( int pageIndex );
private:
    wxToolBar *toolbar;
    HtmlHelp *help;
    wxNotebook *nb;


    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnNew( wxCommandEvent& event );
    void OnOpen( wxCommandEvent& event );
    void OnClose( wxCommandEvent& event );
    void OnRun( wxCommandEvent& event );
    void OnExe( wxCommandEvent& event );
    void OnHome( wxCommandEvent& event );
    void OnBack( wxCommandEvent& event );
    void OnForward( wxCommandEvent& event );

    void OnPageChanged( wxBookCtrlEvent& event );

    wxDECLARE_EVENT_TABLE();
};

#endif
