#ifndef BLITZCC_H
#define BLITZCC_H

#include <wx/thread.h>
#include <wx/process.h>

wxDECLARE_EVENT( BUILD_BEGIN,wxCommandEvent );
wxDECLARE_EVENT( BUILD_PROGRESS,wxCommandEvent );
wxDECLARE_EVENT( BUILD_END,wxCommandEvent );
wxDECLARE_EVENT( BUILD_KILL,wxCommandEvent );

struct Target{
	wxString name,id;
	wxString platform;
	bool host,emulator,running;
};

class BlitzCC:public wxThread{
protected:
	virtual ExitCode Entry();

	wxEvtHandler *dest;
	wxString blitzpath,path;
	Target target;
	wxProcess *proc;

	int Monitor();
public:
	BlitzCC( wxEvtHandler *dest,const wxString &blitzpath );
	~BlitzCC();

	void Execute( const wxString &path,const Target &target );

	void Kill();
};

#endif
