#ifndef BB_INPUT_COMMANDS_H
#define BB_INPUT_COMMANDS_H

#include <bb/blitz/module.h>
#include <bb/input/input.h>

#ifdef __cplusplus
extern "C" {
#endif

// AUTOGENERATED. DO NOT EDIT.
// RUN `make` TO UPDATE.

//keyboard
bb_int_t BBCALL bbKeyDown( bb_int_t key );
bb_int_t BBCALL bbKeyHit( bb_int_t key );
bb_int_t BBCALL bbGetKey( bb_int_t ascii );
bb_int_t BBCALL bbWaitKey(  );
void BBCALL bbFlushKeys(  );

//mouse
bb_int_t BBCALL bbMouseDown( bb_int_t button );
bb_int_t BBCALL bbMouseHit( bb_int_t button );
bb_int_t BBCALL bbGetMouse(  );
bb_int_t BBCALL bbWaitMouse(  );
bb_int_t BBCALL bbWaitMouse(  );
bb_int_t BBCALL bbMouseX(  );
bb_int_t BBCALL bbMouseY(  );
bb_int_t BBCALL bbMouseZ(  );
bb_int_t BBCALL bbMouseXSpeed(  );
bb_int_t BBCALL bbMouseYSpeed(  );
bb_int_t BBCALL bbMouseZSpeed(  );
void BBCALL bbFlushMouse(  );

//joysticks
bb_int_t BBCALL bbCountJoys(  );
bb_int_t BBCALL bbFindJoy( BBStr *id );
BBStr * BBCALL bbJoyID( bb_int_t port );
BBStr * BBCALL bbJoyName( bb_int_t port );
bb_int_t BBCALL bbJoyType( bb_int_t port );
bb_int_t BBCALL bbJoyDown( bb_int_t button,bb_int_t port );
bb_int_t BBCALL bbJoyHit( bb_int_t button,bb_int_t port );
bb_int_t BBCALL bbGetJoy( bb_int_t port );
bb_int_t BBCALL bbWaitJoy( bb_int_t port );
bb_int_t BBCALL bbWaitJoy( bb_int_t port );
bb_float_t BBCALL bbJoyX( bb_int_t port );
bb_float_t BBCALL bbJoyY( bb_int_t port );
bb_float_t BBCALL bbJoyZ( bb_int_t port );
bb_float_t BBCALL bbJoyU( bb_int_t port );
bb_float_t BBCALL bbJoyV( bb_int_t port );
bb_float_t BBCALL bbJoyPitch( bb_int_t port );
bb_float_t BBCALL bbJoyYaw( bb_int_t port );
bb_float_t BBCALL bbJoyRoll( bb_int_t port );
bb_int_t BBCALL bbJoyHat( bb_int_t port );
bb_int_t BBCALL bbJoyXDir( bb_int_t port );
bb_int_t BBCALL bbJoyYDir( bb_int_t port );
bb_int_t BBCALL bbJoyZDir( bb_int_t port );
bb_int_t BBCALL bbJoyUDir( bb_int_t port );
bb_int_t BBCALL bbJoyVDir( bb_int_t port );
void BBCALL bbFlushJoy(  );

#ifdef __cplusplus
}
#endif


#endif
