#ifndef BB_UNIT_TEST_COMMANDS_H
#define BB_UNIT_TEST_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C" {
#endif

void BBCALL __bbContext( const char *mesg, const char *file,int line );
void BBCALL __bbExpect( int condition, const char *mesg, const char *file,int line );
void BBCALL __bbExpectIntEq( int a,int b,const char *mesg, const char *file,int line );
void BBCALL __bbExpectFloatEq( float a,float b,const char *mesg, const char *file,int line );

void BBCALL _bbContext( BBStr *m,const char *file,int line );
void BBCALL _bbExpect( int condition,BBStr *mesg,const char *file,int line );
void BBCALL _bbExpectIntEq( int a,int b,BBStr *mesg,const char *file,int line );
void BBCALL _bbExpectFloatEq( float a,float b,BBStr *mesg,const char *file,int line );

#ifndef BB_WINDOWS
	#define bbContext( mesg ) _bbContext( mesg,__FILE__,__LINE__ )
	#define bbExpect( condition,mesg ) _bbExpect( condition,mesg,__FILE__,__LINE__ )
	#define bbExpectIntEq( a,b,mesg ) _bbExpectIntEq( a,b,mesg,__FILE__,__LINE__ )
	#define bbExpectFloatEq( a,b,mesg ) _bbExpectFloatEq( a,b,mesg,__FILE__,__LINE__ )
#else
	#ifndef USERLIB
		void BBCALL bbContext( BBStr *m );
		void BBCALL bbExpect( int condition,BBStr *m );
		void BBCALL bbExpectIntEq( int a,int b,BBStr *m );
		void BBCALL bbExpectFloatEq( float a,float b,BBStr *m );
	#else
		#define BBEXPORT __declspec(dllexport)

		BBEXPORT void BBCALL bbContext( const char *m );
		BBEXPORT void BBCALL bbExpect( int condition,const char *m );
		BBEXPORT void BBCALL bbExpectIntEq( int a,int b,const char *m );
		BBEXPORT void BBCALL bbExpectFloatEq( float a,float b,const char *m );
	#endif
#endif

#ifndef BBEXPORT
#define BBEXPORT
#endif

#ifdef __cplusplus
}
#endif

#endif
