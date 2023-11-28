#include "../stdutil/stdutil.h"
#include "input.nx.h"
#include <switch.h>
#include <limits.h>

class NXInputDriver : public BBInputDriver{
public:
	~NXInputDriver(){}

	BBDevice *getJoystick( int port )const{ return 0; }
	int getJoystickType( int port )const{ return 0; }
	int numJoysticks()const{ return 0; }

	int toAscii( int key )const{
		return 0;
	}
};

class NXJoystick : public BBDevice{
private:
	PadState pad;

public:
	NXJoystick(){
		reset();
	}

	void initDefault(){
		padInitializeDefault( &pad );
	}

	void update(){
		padUpdate( &pad );

		u64 kDown = padGetButtonsDown( &pad );
		setDownState( 0,kDown&HidNpadButton_Plus );
		setDownState( 1,kDown&HidNpadButton_ZR );
		setDownState( 2,kDown&HidNpadButton_B );

		HidAnalogStickState analog_stick_l = padGetStickPos( &pad,0 );
		axis_states[0]=analog_stick_l.x/(float)SHRT_MAX;
		axis_states[1]=analog_stick_l.y/(float)SHRT_MAX;
	}
};

BBMODULE_CREATE( input_nx ){
	if( !gx_input ){
		padConfigureInput(1, HidNpadStyleSet_NpadStandard);

		gx_input=d_new NXInputDriver();

		NXJoystick *js=d_new NXJoystick();
		js->initDefault();
		bbJoysticks.push_back( js );
	}
	return true;
}

BBMODULE_DESTROY( input_nx ){
	return true;
}
