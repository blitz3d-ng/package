#ifndef BBMODULE_H
#define BBMODULE_H

#include "../config.h"

#undef CDECL

#ifdef WIN32
#define BBDECL __declspec(dllexport)
#define BBCALL __stdcall
#define CDECL __cdecl
#else
#define BBDECL
#define BBCALL
#define CDECL
#endif

#define BBLIB extern "C"

#define BBMODULE_FUNC_NAME( name,action ) name##_##action

#define BBMODULE_CREATE( name )  bool BBMODULE_FUNC_NAME( name, create )()
#define BBMODULE_DESTROY( name ) bool BBMODULE_FUNC_NAME( name, destroy )()
#define BBMODULE_LINK( name )    void BBMODULE_FUNC_NAME( name, link )( void (*_rtSym)( const char *sym,void *pc ) )

#define rtSym( text,sym ) (_rtSym( text,(void*)sym ))

#define BBMODULE_DECL( name ) \
	BBMODULE_CREATE( name ); \
	BBMODULE_DESTROY( name ); \
	BBMODULE_LINK( name );

#define BBMODULE_EMPTY( name ) \
	BBMODULE_CREATE( name ) { return true; } \
	BBMODULE_DESTROY( name ) { return true; } \
	BBMODULE_LINK( name ) { }

#endif
