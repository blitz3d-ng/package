
#include <string>
#include <windows.h>

#include "checkdx.h"
#include "checkie.h"

using namespace std;

static const wchar_t *dx_err=
L"You must have DirectX version 7 installed to run Blitz Basic.\n\n"
"DirectX 7 is provided on the Blitz Basic CD in the DirectX7 folder.\n\n"
"The latest version of DirectX is available from www.microsoft.com";

static const wchar_t *ie_err=
L"You must have Internet Explorer version 4 installed to run Blitz Basic.\n\n"
"Internet Explorer 5 is provided on the Blitz Basic CD in the IExplorer5 folder.\n\n"
"The latest version of Internet Explorer is available from www.microsoft.com";

static const wchar_t *bb_err=
L"Unable to run Blitz Basic";

static const wchar_t *md_err=
L"Your desktop must be in high-colour mode to use Blitz Basic.\n\n"
"You can change your display settings from the control panel.";

static wstring getAppDir(){
	wchar_t buff[MAX_PATH];
	if( GetModuleFileName( 0,buff,MAX_PATH ) ){
		wstring t=buff;
		size_t n=t.find_last_of( '\\' );
		if( n!=string::npos ) t=t.substr( 0,n );
		return t;
	}
	return L"";
}

static void fail( const wchar_t *p ){
	::MessageBox( 0,p,L"Blitz Basic Error",MB_SETFOREGROUND|MB_TOPMOST|MB_ICONERROR );
	ExitProcess(-1);
}

static int desktopDepth(){
	HDC hdc=GetDC( GetDesktopWindow() );
	return GetDeviceCaps( hdc,BITSPIXEL );
}

int __stdcall wWinMain( HINSTANCE inst,HINSTANCE prev,wchar_t *cmd,int show ){

	if( desktopDepth()<16 ) fail( md_err );

#ifndef PLUS
	if( getDXVersion()<7 ) fail( dx_err );
#endif

	if( getIEVersion()<4 ) fail( ie_err );

	//Ugly hack to get application dir...
	wstring t=getAppDir();
	_wputenv( (L"blitzpath="+t).c_str() );
	SetCurrentDirectory( t.c_str() );
	t=t+L"\\bin\\ide" TARGETSUFFIX ".exe "+wstring(cmd);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(si));si.cb=sizeof(si);
	if( !CreateProcess( 0,(LPWSTR)t.c_str(),0,0,0,0,0,0,&si,&pi ) ){
		::MessageBox( 0,bb_err,L"Blitz Basic Error",MB_SETFOREGROUND|MB_TOPMOST|MB_ICONERROR );
		ExitProcess(-1);
	}

	//wait for BB to start
	WaitForInputIdle( pi.hProcess,INFINITE );

	// Close process and thread handles.
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return 0;
}

#ifdef __MINGW32__
int __stdcall WinMain( HINSTANCE inst,HINSTANCE prev,char *cmd,int show ){
	wchar_t *w_cmd=GetCommandLineW();
	return wWinMain( inst,prev,w_cmd,show );
}
#endif
