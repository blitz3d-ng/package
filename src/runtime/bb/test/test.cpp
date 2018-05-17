
#include <bb/blitz/blitz.h>
#include "../../stdutil/stdutil.h"
#include "test.h"

using namespace std;

void BBCALL _bbExpect( int condition,BBStr *m, const char *file,int line ){
	string mesg=*m;delete m;
	if( !condition ){
		cerr << "\033[1;31m" << "FAIL: " << "\033[0m" << mesg << ". [" << file << ":" << line << "]" << endl;
	} else {
		cerr << "\033[1;32m" << "PASS: " << "\033[0m" << mesg << ". [" << file << ":" << line << "]" << endl;
	}
}

BBMODULE_CREATE( test ){
	return true;
}

BBMODULE_DESTROY( test ){
	return true;
}
