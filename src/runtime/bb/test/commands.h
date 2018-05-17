#ifndef BB_TEST_COMMANDS_H
#define BB_TEST_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C" {
#endif

void BBCALL _bbExpect( int condition,BBStr *mesg,const char *file,int line );

#define bbExpect( condition,mesg ) _bbExpect( condition,mesg,__FILE__,__LINE__ )

#ifdef __cplusplus
}
#endif

#endif
