#ifndef BB_UNIT_TEST_COMMANDS_H
#define BB_UNIT_TEST_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C" {
#endif

void BBCALL _bbContext( BBStr *mesg,const char *file,int line );
void BBCALL _bbExpect( int condition,BBStr *mesg,const char *file,int line );

#ifndef WIN32
#define bbContext( mesg ) _bbContext( mesg,__FILE__,__LINE__ )
#define bbExpect( condition,mesg ) _bbExpect( condition,mesg,__FILE__,__LINE__ )
#else
void BBCALL bbContext( BBStr *m );
void BBCALL bbExpect( int condition,BBStr *m );
#endif

#ifdef __cplusplus
}
#endif

#endif
