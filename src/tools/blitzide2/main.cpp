#include <wx/wxprec.h>
#include <wx/cmdline.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include "MainFrame.h"
#include "dpi.h"

wxString blitzpath;

class MyApp : public wxApp{
private:
	MainFrame *frame;

	wxArrayString filesToOpen;
public:
	virtual bool OnInit();

	virtual void OnInitCmdLine( wxCmdLineParser& parser );
	virtual bool OnCmdLineParsed( wxCmdLineParser& parser );
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit(){
	if( !wxApp::OnInit() ){
		return false;
	}

	if( !wxGetEnv( "blitzpath",&blitzpath ) || blitzpath.length()==0 ){
		wxMessageBox( "blitzpath not set", "Blitz3D \"NG\"",wxOK|wxICON_ERROR );
		return false;
	}

	frame = new MainFrame( "Blitz3D \"NG\"" );
	frame->SetClientSize( frame->FromDIP( wxSize( 800,600 ) ) );
	frame->AddFiles( filesToOpen );
	frame->Show( true );
	return true;
}

void MyApp::OnInitCmdLine( wxCmdLineParser& parser ){
	parser.SetSwitchChars( wxT("-") );
	parser.AddParam( wxT("input files"),wxCMD_LINE_VAL_STRING,wxCMD_LINE_PARAM_MULTIPLE|wxCMD_LINE_PARAM_OPTIONAL );
}

bool MyApp::OnCmdLineParsed( wxCmdLineParser& parser ){
	for( int i=0;i<parser.GetParamCount();i++ ){
		wxString param=parser.GetParam(i);
		filesToOpen.Add( param );
	}
	return true;
}
