
#ifndef GXINPUT_H
#define GXINPUT_H

#include <bb/blitz/module.h>
#include <bb/input/input.h>
#include <dinput.h>

class gxRuntime;

class DirectInput8Driver : public BBInputDriver{
public:
	IDirectInput8 *dirInput;

	DirectInput8Driver( IDirectInput8 *di );
	~DirectInput8Driver();

	void reset();

private:

	/***** GX INTERFACE *****/
public:

	BBDevice *getJoystick( int port )const;
	int getJoystickType( int port )const;
	int numJoysticks()const;
	int toAscii( int key )const;
};

void BBCALL bbEnableDirectInput( int enable );
int  BBCALL bbDirectInputEnabled();

#endif
