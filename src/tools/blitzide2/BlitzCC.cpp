#include "BlitzCC.h"
#include <wx/sstream.h>
#include <wx/txtstrm.h>
#include <wx/msgdlg.h>
#include <iostream>

wxDEFINE_EVENT( BUILD_BEGIN,wxCommandEvent );
wxDEFINE_EVENT( BUILD_PROGRESS,wxCommandEvent );
wxDEFINE_EVENT( BUILD_END,wxCommandEvent );
wxDEFINE_EVENT( BUILD_KILL,wxCommandEvent );

BlitzCC::BlitzCC( wxEvtHandler *dest,const wxString &blitzpath ):dest(dest),blitzpath(blitzpath),proc(0){
}

BlitzCC::~BlitzCC(){
}

void BlitzCC::Execute( const wxString &p,const Target &t ){
	path=p;
	target=t;
	Run();
}

void BlitzCC::Kill(){
	if( proc==0 ) return;

	wxProcess::Kill( proc->GetPid() );
}

int BlitzCC::Monitor(){
	if( proc==0 ) return 0;

	wxTextInputStream outs( *proc->GetInputStream() );
	wxString lines;
	while( proc->IsInputOpened() ){
		wxString line=outs.ReadLine();

		if( line.size()>0 ){
			wxCommandEvent event( BUILD_PROGRESS );
			event.SetString( line );
			wxPostEvent( dest,event );
		}
	}

	return 0;
}

wxThread::ExitCode BlitzCC::Entry(){
	wxPostEvent( dest,wxCommandEvent( BUILD_BEGIN ) );


	wxFileName out( path );
	out.ClearExt();
	out.SetExt( "app" );
	out.Normalize();

	wxString signId="";
	wxString teamId="";
	wxString args;
	if( !target.host ){
		args += " -c -sign "+signId;
		if( target.platform=="ios" ){ // only need entitlement when deploying to real device
			args +=" -team "+teamId;
		}
		args += " -target "+target.platform+" -o "+out.GetFullPath();
	}
	args += " "+path;

	wxString bundleId;

	proc=new wxProcess( 0 );
	proc->Redirect();
	wxExecute( blitzpath+"/bin/blitzcc"+args,wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE,proc );
	wxTextInputStream outs( *proc->GetInputStream() );
	wxString lines;
	while( proc->IsInputOpened() ){
		wxString line=outs.ReadLine();

		if( line.size()>0 ){
			wxCommandEvent event( BUILD_PROGRESS );
			event.SetString( line );
			wxPostEvent( dest,event );

			// TODO: avoid this dirty hack...
			if( line.StartsWith("Signing ") && line.EndsWith("...") ){
				bundleId=line.Mid( wxString("Signing ").size() );
				bundleId=bundleId.Mid( 0,bundleId.size()-3 );
			}
		}
	}

	std::cout<<bundleId<<std::endl;

	if( target.platform=="ios-sim" ){
		proc=new wxProcess( 0 );
		proc->Redirect();
		wxExecute( "xcrun simctl install "+target.id+" "+out.GetFullPath(),wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE,proc );
		Monitor();

		proc=new wxProcess( 0 );
		proc->Redirect();
		wxExecute( "xcrun simctl launch --console --terminate-running-process "+target.id+" "+bundleId,wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE,proc );
		Monitor();
	}else if( target.platform=="ios" ){
		proc=new wxProcess( 0 );
		proc->Redirect();
		wxExecute( "ios-deploy --bundle "+out.GetFullPath(),wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE,proc );
		Monitor();
	}

	wxPostEvent( dest,wxCommandEvent( BUILD_END ) );

	return (wxThread::ExitCode)0;
}
