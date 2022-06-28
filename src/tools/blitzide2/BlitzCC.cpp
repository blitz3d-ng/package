#include "BlitzCC.h"
#include <wx/sstream.h>
#include <wx/msgdlg.h>
#include <iostream>

BlitzCC::BlitzCC( const wxString &blitzpath ):blitzpath(blitzpath){
}

BlitzCC::~BlitzCC(){
}

void BlitzCC::Execute( const wxString &path ){
	args=path;
	Run();
}

wxThread::ExitCode BlitzCC::Entry(){
	wxProcess *run=new wxProcess( 0 );
	run->Redirect();
	wxExecute( blitzpath+"/bin/blitzcc "+args,wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE,run );

	// wxInputStream *outs=run->GetInputStream();

	// wxString lines;
	while( !TestDestroy() ){
		wxStringOutputStream stream;
		// outs->Read( stream );

	}

	// std::cout<<lines<<std::endl;


  return (wxThread::ExitCode)0;
}
