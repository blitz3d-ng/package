#ifdef BB_WINDOWS
#include <windows.h>
#endif
#include <string>
#include <cstdlib>

#ifdef BB_WINDOWS
static string getAppDir(){
	char buff[MAX_PATH];
	if( GetModuleFileName( 0,buff,MAX_PATH ) ){
		string t=buff;
		size_t n=t.find_last_of( '\\' );
		if( n!=string::npos ) t=t.substr( 0,n );
		return t;
	}
	return "";
}

int WinMain( HINSTANCE inst,HINSTANCE prev,char *cmd,int show )
#else
#include <libgen.h>

int main( int argc,char **argv )
#endif
{
#ifdef BB_WINDOWS
	string t=getAppDir();
	putenv( ("blitzpath="+t).c_str() );
	SetCurrentDirectory( t.c_str() );
	t=t+"\\bin\\ide2.exe "+string(cmd);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(si));si.cb=sizeof(si);
	if( !CreateProcess( 0,(LPSTR)t.c_str(),0,0,0,0,0,0,&si,&pi ) ){
		::MessageBox( 0,"Unable to run Blitz Basic","Blitz Basic Error",MB_SETFOREGROUND|MB_TOPMOST|MB_ICONERROR );
		ExitProcess(-1);
	}

	//wait for BB to start
	WaitForInputIdle( pi.hProcess,INFINITE );

	// Close process and thread handles.
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
#else
	std::string blitzpath;
	blitzpath=std::string( dirname( argv[0] ) )+"/../../../";

	setenv( "blitzpath",blitzpath.c_str(),1 );
	system( (blitzpath+"/bin/ide2").c_str() );
#endif

	return 0;
}
