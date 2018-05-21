
#include "module.h"
#include "ex.h"
#include <cstring>

extern "C" void BBCALL _bbDebugError( const char *e );

bbEx::bbEx( const char *e ){
	err=e?strdup( e ):0;
	if( err ) _bbDebugError( err );
}

bbEx::~bbEx(){
	delete err;
}
