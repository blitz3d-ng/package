#ifndef BB_INPUT_COMMANDS_H
#define BB_INPUT_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C" {
#else
typedef void BBFont;
#endif

//keyboard
int  BBCALL bbKeyDown( int n );
int  BBCALL bbKeyHit( int n );
int  BBCALL bbGetKey( int ascii );
int  BBCALL bbWaitKey();
void BBCALL bbFlushKeys();

//mouse
int  BBCALL bbMouseDown( int n );
int  BBCALL bbMouseHit( int n );
int  BBCALL bbGetMouse();
int  BBCALL bbWaitMouse();
int  BBCALL bbMouseX();
int  BBCALL bbMouseY();
int  BBCALL bbMouseZ();
int  BBCALL bbMouseXSpeed();
int  BBCALL bbMouseYSpeed();
int  BBCALL bbMouseZSpeed();
void BBCALL bbFlushMouse();

//joysticks
int   BBCALL bbCountJoys();
int   BBCALL bbFindJoy( BBStr *id );
BBStr * BBCALL bbJoyID( int port );
BBStr * BBCALL bbJoyName( int port );
int		BBCALL bbJoyType( int port );
int		BBCALL bbJoyDown( int n,int port );
int		BBCALL bbJoyHit( int n,int port );
int		BBCALL bbGetJoy( int port );
int		BBCALL bbWaitJoy( int port );
float	BBCALL bbJoyX( int port );
float	BBCALL bbJoyY( int port );
float	BBCALL bbJoyZ( int port );
float	BBCALL bbJoyU( int port );
float	BBCALL bbJoyV( int port );
float	BBCALL bbJoyPitch( int port );
float	BBCALL bbJoyYaw( int port );
float	BBCALL bbJoyRoll( int port );
int		BBCALL bbJoyHat( int port );
int		BBCALL bbJoyXDir( int port );
int		BBCALL bbJoyYDir( int port );
int		BBCALL bbJoyZDir( int port );
int		BBCALL bbJoyUDir( int port );
int		BBCALL bbJoyVDir( int port );
void	BBCALL bbFlushJoy();

#ifdef __cplusplus
}
#endif

#endif
