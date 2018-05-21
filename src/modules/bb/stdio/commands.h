#ifndef BB_STDIO_COMMANDS_H
#define BB_STDIO_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C" {
#else
#endif

void BBCALL bbWriteStdout( BBStr *str );
void BBCALL bbWriteStderr( BBStr *str );

#ifdef __cplusplus
}
#endif

#endif
