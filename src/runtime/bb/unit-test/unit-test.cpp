
#include <bb/blitz/blitz.h>
#include "../../stdutil/stdutil.h"
#include "unit-test.h"

using namespace std;

static int _bbPasses, _bbFails;

void BBCALL _bbContext( BBStr *m, const char *file,int line ){
	string mesg=*m;delete m;
	cout << mesg << " [" << file << ":" << line << "]" << endl;
}

void BBCALL _bbExpect( int condition,BBStr *m, const char *file,int line ){
	string mesg=*m;delete m;
	if( !condition ){
		cout << "\033[1;31m" << "FAIL: " << "\033[0m" << mesg << ". [" << file << ":" << line << "]" << endl;
		_bbFails++;
	} else {
		cout << "\033[1;32m" << "PASS: " << "\033[0m" << mesg << ". [" << file << ":" << line << "]" << endl;
		_bbPasses++;
	}
}

#ifdef WIN32
void BBCALL bbContext( BBStr *m ){
	_bbContext( m, "<unknown>", 0 );
}

void BBCALL bbExpect( int condition,BBStr *m ){
	_bbExpect( condition,m,"<unknown>",0 );
}
#endif

BBMODULE_CREATE( unit_test ){
	_bbPasses = 0;
	_bbFails = 0;
	return true;
}

BBMODULE_DESTROY( unit_test ){
	cout << endl << "==== Results ====" << endl;
	cout << "Pass: " << "\033[1;32m" << _bbPasses << "\033[0m. Fail: " << "\033[1;31m" << _bbFails << "\033[0m." << endl;

	if (_bbFails > 0) {
		exit(1);
	}
	return true;
}
