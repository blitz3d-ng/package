#include "unit-test.h"

#define BBEXPORT BBDECL

BBEXPORT void BBCALL bbContext( const char *m ){
	bbContext( m );
}

BBEXPORT void BBCALL bbExpect( int condition,const char *m ){
	bbExpect( condition,m );
}

BBEXPORT void BBCALL bbExpectIntEq( int a,int b,const char *m ){
	bbExpectIntEq( a,b,m );
}

BBEXPORT void BBCALL bbExpectFloatEq( float a,float b,const char *m ){
	bbExpectIntEq( a,b,m );
}

// TODO: make this unneccesary.
bool bbRuntimeIdle() {
	return true;
}

#include <windows.h>

BBMODULE_DECL( unit_test );

BOOL APIENTRY DllMain( HMODULE module,DWORD reason,LPVOID reserved ){
	switch( reason ){
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		unit_test_create();
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		unit_test_destroy();
		break;
	}
	return TRUE;
}
