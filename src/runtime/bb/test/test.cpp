
#include <bb/blitz/blitz.h>
#include "../../stdutil/stdutil.h"
#include "test.h"

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

BBMODULE_CREATE( test ){
	_bbPasses = 0;
	_bbFails = 0;
	return true;
}

BBMODULE_DESTROY( test ){
	cout << endl << "==== Results ====" << endl;
	cout << "Pass: " << "\033[1;32m" << _bbPasses << "\033[0m. Fail: " << "\033[1;31m" << _bbFails << "\033[0m." << endl;

	return true;
}
