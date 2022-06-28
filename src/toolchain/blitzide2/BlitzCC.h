#ifndef BLITZCC_H
#define BLITZCC_H

#include <wx/thread.h>
#include <wx/process.h>

class BlitzCC:public wxThread{
protected:
	virtual ExitCode Entry();

	wxString blitzpath,args;
	wxProcess *proc;
public:
	BlitzCC( const wxString &blitzpath );
	~BlitzCC();

	void Execute( const wxString &path );
};

#endif
