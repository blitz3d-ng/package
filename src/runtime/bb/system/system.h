#ifndef BB_SYSTEM_SYSTEM_H
#define BB_SYSTEM_SYSTEM_H

#include <bb/blitz/blitz.h>

BBStr * BBCALL bbGetEnv( BBStr *env_var );
void BBCALL bbSetEnv( BBStr *env_var,BBStr *val );
int BBCALL bbScreenWidth( int i );
int BBCALL bbScreenHeight( int i );

#endif
