
#include <bb/blitz/blitz.h>
#include <bb/math/math.h>
#include "../stdutil/stdutil.h"
#include "unit-test.h"

static int _bbPasses, _bbFails;

static std::string GREEN="\033[1;32m",RED="\033[1;31m",CLEAR="\033[0m";

#define FAIL(mesg) std::cout << RED << "FAIL: " << CLEAR << mesg << ". [" << file << ":" << line << "]" << std::endl;_bbFails++
#define PASS(mesg) std::cout << GREEN << "PASS: " << CLEAR << mesg << ". [" << file << ":" << line << "]" << std::endl;_bbPasses++

void BBCALL __bbContext( const char *mesg, const char *file,int line ){
	std::cout << mesg << " [" << file << ":" << line << "]" << std::endl;
}

void BBCALL __bbExpect( int condition, const char *mesg, const char *file,int line ){
	if( !condition ){
		FAIL(mesg);
	} else {
		PASS(mesg);
	}
}

void BBCALL __bbExpectInt( int a,int b,const char *mesg, const char *file,int line ){
	if( a!=b ){
		FAIL(mesg << ". Expected: " << b << ", but got " << a);
	} else {
		PASS(mesg);
	}
}

const float EPSILON=.000001f;		//small value

void BBCALL __bbExpectFloat( float a,float b,const char *mesg, const char *file,int line ){
	if( abs(a-b)>EPSILON ){
		FAIL(mesg << ". Expected: " << b << ", but got " << a);
	} else {
		PASS(mesg);
	}
}

void BBCALL __bbExpectStr( std::string& a,std::string &b,const char *mesg, const char *file,int line ){
	if( a!=b ){
		FAIL(mesg << ". Expected: '" << b << "', but got '"<<a<<"'");
	} else {
		if( strlen(mesg)>0 ){
			PASS(mesg);
		}else{
			PASS("'"+a+"' matches '"+b+"'");
		}
	}
}

void BBCALL _bbContext( BBStr *m, const char *file,int line ){
	std::string mesg=*m;delete m;
	__bbContext( mesg.c_str(),file,line );
}

void BBCALL _bbExpect( bb_int_t condition,BBStr *m, const char *file,int line ){
	std::string mesg=*m;delete m;
	__bbExpect( condition,mesg.c_str(),file,line );
}

void BBCALL _bbExpectInt( bb_int_t a,bb_int_t b,BBStr *m, const char *file,int line ){
	std::string mesg=*m;delete m;
	__bbExpectFloat( a,b,mesg.c_str(),file,line );
}

void BBCALL _bbExpectFloat( bb_float_t a,bb_float_t b,BBStr *m, const char *file,int line ){
	std::string mesg=*m;delete m;
	__bbExpectFloat( a,b,mesg.c_str(),file,line );
}

void BBCALL _bbExpectStr( BBStr *a,BBStr *b,BBStr *m, const char *file,int line ){
	std::string mesg=*m;delete m;
	__bbExpectStr( *a,*b,mesg.c_str(),file,line );
	delete a;delete b;
}

void BBCALL bbContext( BBStr *m ){
	_bbContext( m, "<unknown>", 0 );
}

void BBCALL bbExpect( bb_int_t condition,BBStr *m ){
	_bbExpect( condition,m,"<unknown>",0 );
}

void BBCALL bbExpectInt( bb_int_t a,bb_int_t b,BBStr *m ){
	_bbExpectInt( a,b,m,"<unknown>",0 );
}

void BBCALL bbExpectFloat( bb_float_t a,bb_float_t b,BBStr *m ){
	_bbExpectFloat( a,b,m,"<unknown>",0 );
}

void BBCALL bbExpectStr( BBStr *a,BBStr *b,BBStr *m ){
	_bbExpectStr( a,b,m,"<unknown>",0 );
}

#ifdef BB_WINDOWS
#include <windows.h>
#include <ios>
#include <cstdio>
#include <io.h>
#include <fcntl.h>

#endif

BBMODULE_CREATE( unit_test ){
	_bbPasses = 0;
	_bbFails = 0;

#ifdef WIN32
	HANDLE handle=GetStdHandle( STD_OUTPUT_HANDLE );
	DWORD mode;
	GetConsoleMode( handle,&mode);
	SetConsoleMode( handle,mode|ENABLE_VIRTUAL_TERMINAL_PROCESSING|DISABLE_NEWLINE_AUTO_RETURN );
#endif

#ifdef USERLIB
	if( AllocConsole() ){
		SetConsoleTitle( "Debug Console" );

		// https://smacdo.com/programming/redirecting-standard-output-to-console-in-windows/

		// Redirect CRT standard input, output and error handles to the console window.
		FILE * pNewStdout = nullptr;
		FILE * pNewStderr = nullptr;
		FILE * pNewStdin = nullptr;

		::freopen_s(&pNewStdout, "CONOUT$", "w", stdout);
		::freopen_s(&pNewStderr, "CONOUT$", "w", stderr);
		::freopen_s(&pNewStdin, "CONIN$", "r", stdin);

		// Clear the error state for all of the C++ standard streams. Attempting to accessing the streams before they refer
		// to a valid target causes the stream to enter an error state. Clearing the error state will fix this problem,
		// which seems to occur in newer version of Visual Studio even when the console has not been read from or written
		// to yet.
		std::cout.clear();
		std::cerr.clear();
		std::cin.clear();

		std::wcout.clear();
		std::wcerr.clear();
		std::wcin.clear();
	}

	printf("%s\n", "Hello, world!");
	std::cout << "Hello, world!" << std::endl;
#endif

	return true;
}

BBMODULE_DESTROY( unit_test ){
	std::cout << std::endl << "==== Results ====" << std::endl;
	std::cout << "Pass: " << GREEN << _bbPasses << CLEAR << ". Fail: " << RED << _bbFails << CLEAR << std::endl;

	if (_bbFails > 0) {
		exit(1);
	}
	return true;
}
