
#ifndef STD_H
#define STD_H

//#ifndef _WINSOCKAPI_
//#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
//#endif
#include <windows.h>
//#include <winsock2.h>

#include "../config/config.h"
#include "../stdutil/stdutil.h"

#include <set>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <math.h>

using namespace std;

#define BBCALL __stdcall

#define BBMODULE_FUNC_NAME( name,action ) name##_##action

#define BBMODULE_CREATE( name )  bool BBMODULE_FUNC_NAME( name, create )()
#define BBMODULE_DESTROY( name ) bool BBMODULE_FUNC_NAME( name, destroy )()
#define BBMODULE_LINK( name )    void BBMODULE_FUNC_NAME( name, link )( void (*rtSym)( const char *sym,void *pc ) )

#define BBMODULE_DECL( name ) \
	BBMODULE_CREATE( name ); \
	BBMODULE_DESTROY( name ); \
	BBMODULE_LINK( name );


#endif
