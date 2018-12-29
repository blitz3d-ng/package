#include "unit-test.h"

BBEXPORT void BBCALL bbContext( const char *m ){
	__bbContext( m, "<unknown>", 0 );
}

BBEXPORT void BBCALL bbExpect( int condition,const char *m ){
	__bbExpect( condition,m,"<unknown>",0 );
}

BBEXPORT void BBCALL bbExpectIntEq( int a,int b,const char *m ){
	__bbExpectIntEq( a,b,m,"<unknown>",0 );
}

BBEXPORT void BBCALL bbExpectFloatEq( float a,float b,const char *m ){
	__bbExpectIntEq( a,b,m,"<unknown>",0 );
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
