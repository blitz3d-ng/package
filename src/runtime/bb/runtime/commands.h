#ifndef BB_RUNTIME_COMMANDS_H
#define BB_RUNTIME_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C" {
#else
#endif

void BBCALL bbRuntimeError( BBStr *str );

void BBCALL bbShowPointer();
void BBCALL bbHidePointer();

#ifdef __cplusplus
}
#endif

#endif
