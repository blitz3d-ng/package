
#ifndef GXINPUT_H
#define GXINPUT_H

#include <bb/blitz/module.h>
#include <bb/input/input.h>
#include <dinput.h>

class gxRuntime;

class DirectInput8Driver : public BBInputDriver{
public:
	gxRuntime *runtime;
	IDirectInput8 *dirInput;

	DirectInput8Driver( gxRuntime *runtime,IDirectInput8 *di );
	~DirectInput8Driver();

	void reset();
	bool acquire();
	void unacquire();

	void wm_keydown( int key );
	void wm_keyup( int key );
	void wm_mousedown( int key );
	void wm_mouseup( int key );
	void wm_mousemove( int x,int y );
	void wm_mousewheel( int dz );

private:

	/***** GX INTERFACE *****/
public:
	void moveMouse( int x,int y );

	BBDevice *getMouse()const;
	BBDevice *getKeyboard()const;
	BBDevice *getJoystick( int port )const;
	int getJoystickType( int port )const;
	int numJoysticks()const;
	int toAscii( int key )const;
};

void BBCALL bbEnableDirectInput( int enable );
int  BBCALL bbDirectInputEnabled();

#endif
