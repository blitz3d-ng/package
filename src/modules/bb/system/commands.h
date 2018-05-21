#ifndef BB_SYSTEM_COMMANDS_H
#define BB_SYSTEM_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C"{
#endif

int BBCALL bbExecFile( BBStr *f );

void BBCALL bbDelay( int ms );
int BBCALL bbMilliSecs();

BBStr * BBCALL bbSystemProperty( BBStr *p );
BBStr * BBCALL bbGetEnv( BBStr *env_var );
void BBCALL bbSetEnv( BBStr *env_var,BBStr *val );

int BBCALL bbScreenWidth( int i );
int BBCALL bbScreenHeight( int i );

float  BBCALL bbDPIScaleX();
float  BBCALL bbDPIScaleY();

#ifdef __cplusplus
}
#endif

#endif
