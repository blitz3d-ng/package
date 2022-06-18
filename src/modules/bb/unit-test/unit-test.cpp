
#include <bb/blitz/blitz.h>
#include <bb/math/math.h>
#include "../../stdutil/stdutil.h"
#include "unit-test.h"

using namespace std;

static int _bbPasses, _bbFails;

#ifndef BB_WINDOWS
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define CLEAR "\033[0m"
#else
// no color support in Windows...
#define GREEN ""
#define RED ""
#define CLEAR ""
#endif

#define FAIL(mesg) cout << RED << "FAIL: " << CLEAR << mesg << ". [" << file << ":" << line << "]" << endl;_bbFails++
#define PASS(mesg) cout << GREEN << "PASS: " << CLEAR << mesg << ". [" << file << ":" << line << "]" << endl;_bbPasses++

void BBCALL __bbContext( const char *mesg, const char *file,int line ){
	cout << mesg << " [" << file << ":" << line << "]" << endl;
}

void BBCALL __bbExpect( int condition, const char *mesg, const char *file,int line ){
	if( !condition ){
		FAIL(mesg);
	} else {
		PASS(mesg);
	}
}

void BBCALL __bbExpectIntEq( int a,int b,const char *mesg, const char *file,int line ){
	if( a!=b ){
		FAIL(mesg << ". Expected: " << b << ", but got " << a);
	} else {
		PASS(mesg);
	}
}

const float EPSILON=.000001f;		//small value

void BBCALL __bbExpectFloatEq( float a,float b,const char *mesg, const char *file,int line ){
	if( abs(a-b)>EPSILON ){
		FAIL(mesg << ". Expected: " << b << ", but got " << a);
	} else {
		PASS(mesg);
	}
}

void BBCALL _bbContext( BBStr *m, const char *file,int line ){
	string mesg=*m;delete m;
	__bbContext( mesg.c_str(),file,line );
}

void BBCALL _bbExpect( int condition,BBStr *m, const char *file,int line ){
	string mesg=*m;delete m;
	__bbExpect( condition,mesg.c_str(),file,line );
}

void BBCALL _bbExpectIntEq( int a,int b,BBStr *m, const char *file,int line ){
	string mesg=*m;delete m;
	__bbExpectFloatEq( a,b,mesg.c_str(),file,line );
}

void BBCALL _bbExpectFloatEq( float a,float b,BBStr *m, const char *file,int line ){
	string mesg=*m;delete m;
	__bbExpectFloatEq( a,b,mesg.c_str(),file,line );
}

void BBCALL bbContext( BBStr *m ){
	_bbContext( m, "<unknown>", 0 );
}

void BBCALL bbExpect( bb_int_t condition,BBStr *m ){
	_bbExpect( condition,m,"<unknown>",0 );
}

void BBCALL bbExpectIntEq( bb_int_t a,bb_int_t b,BBStr *m ){
	_bbExpectIntEq( a,b,m,"<unknown>",0 );
}

void BBCALL bbExpectFloatEq( bb_float_t a,bb_float_t b,BBStr *m ){
	_bbExpectIntEq( a,b,m,"<unknown>",0 );
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
	cout << "Hello, world!" << endl;
#endif

	return true;
}

BBMODULE_DESTROY( unit_test ){
	cout << endl << "==== Results ====" << endl;
	cout << "Pass: " << GREEN << _bbPasses << CLEAR << ". Fail: " << RED << _bbFails << CLEAR << endl;

	if (_bbFails > 0) {
		exit(1);
	}
	return true;
}
