
#include "../stdutil/stdutil.h"

#include <bb/runtime/runtime.h>
#include <bb/system/system.h>
#include <bb/input/input.h>
#include "driver.h"

#include <math.h>
#include <vector>

// ugly hack to avoid conflicts with GLFW
#include <initguid.h>
DEFINE_GUID(IID_IDirectInput8A, 0xBF798030,0x483A,0x4DA2,0xAA,0x99,0x5D,0x64,0xED,0x36,0x97,0x00);
DEFINE_GUID(IID_IDirectInput8W, 0xBF798031,0x483A,0x4DA2,0xAA,0x99,0x5D,0x64,0xED,0x36,0x97,0x00);

static const int QUE_SIZE=32;

class Device : public BBDevice{
public:
	bool acquired;
	DirectInput8Driver *input;
	IDirectInputDevice8 *device;

	Device( DirectInput8Driver *i,IDirectInputDevice8 *d ):input(i),acquired(false),device(d){
	}
	virtual ~Device(){
		device->Release();
	}
	bool acquire(){
		return acquired=device->Acquire()>=0;
	}
	void unacquire(){
		device->Unacquire();
		acquired=false;
	}
};

class Joystick : public Device{
public:
	int type,poll_time;
	int mins[12],maxs[12];
	Joystick( DirectInput8Driver *i,IDirectInputDevice8 *d,int t ):Device(i,d),type(t),poll_time(0){
		DIDEVICEINSTANCE info;
		info.dwSize=sizeof(DIDEVICEINSTANCE);
		if( d->GetDeviceInfo( &info )==DI_OK ){
			snprintf( id,sizeof(id),"%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
	      info.guidInstance.Data1, info.guidInstance.Data2, info.guidInstance.Data3,
	      info.guidInstance.Data4[0], info.guidInstance.Data4[1], info.guidInstance.Data4[2], info.guidInstance.Data4[3],
	      info.guidInstance.Data4[4], info.guidInstance.Data4[5], info.guidInstance.Data4[6], info.guidInstance.Data4[7]);
			snprintf( name,sizeof(name),"%s",info.tszProductName );
		}

		for( int k=0;k<12;++k ){
			//initialize joystick axis ranges (d'oh!)
			DIPROPRANGE range;
			range.diph.dwSize=sizeof(DIPROPRANGE);
			range.diph.dwHeaderSize=sizeof(DIPROPHEADER);
			range.diph.dwObj=k*4+12;
			range.diph.dwHow=DIPH_BYOFFSET;
			if( d->GetProperty( DIPROP_RANGE,&range.diph )<0 ){
				mins[k]=0;
				maxs[k]=65535;
				continue;
			}
			mins[k]=range.lMin;
			maxs[k]=range.lMax-range.lMin;
		}
	}
	void update(){
		unsigned tm=timeGetTime();
		if( tm-poll_time<3 ) return;
		if( device->Poll()<0 ){
			acquired=false;
			bbRuntimeIdle();
			acquire();if( device->Poll()<0 ) return;
		}
		poll_time=tm;
		DIJOYSTATE state;
		if( device->GetDeviceState( sizeof( state ),&state )<0 ) return;
		axis_states[0]=(state.lX-mins[0])/(float)maxs[0]*2-1;
		axis_states[1]=(state.lY-mins[1])/(float)maxs[1]*2-1;
		axis_states[2]=(state.lZ-mins[2])/(float)maxs[2]*2-1;
		axis_states[3]=(state.rglSlider[0]-mins[6])/(float)maxs[6]*2-1;
		axis_states[4]=(state.rglSlider[1]-mins[7])/(float)maxs[7]*2-1;
		axis_states[5]=(state.lRx-mins[3])/(float)maxs[3]*2-1;
		axis_states[6]=(state.lRy-mins[4])/(float)maxs[4]*2-1;
		axis_states[7]=(state.lRz-mins[5])/(float)maxs[5]*2-1;
		if( (state.rgdwPOV[0]&0xffff)==0xffff ) axis_states[8]=-1;
		else axis_states[8]=floor(state.rgdwPOV[0]/100.0f+.5f);

		for( int k=0;k<31;++k ){
			setDownState( k+1,state.rgbButtons[k]&0x80 ? true : false );
		}
	}
};

static std::vector<Joystick*> joysticks;

static Joystick *createJoystick( DirectInput8Driver *input,LPCDIDEVICEINSTANCE devinst ){
	IDirectInputDevice8 *dev;
	if( input->dirInput->CreateDevice( devinst->guidInstance,&dev,0 )>=0 ){
		if( dev->SetCooperativeLevel( (HWND)bbRuntimeWindow(),DISCL_FOREGROUND|DISCL_NONEXCLUSIVE )>=0 ){
			if( dev->SetDataFormat( &c_dfDIJoystick )>=0 ){
				int t=((devinst->dwDevType>>8)&0xff)==DI8DEVCLASS_GAMECTRL ? 1 : 2;
				return d_new Joystick( input,dev,t );
			}
		}
		dev->Release();
	}
	return 0;
}

static BOOL CALLBACK enumJoystick( LPCDIDEVICEINSTANCE devinst,LPVOID pvRef ){
	// int type=devinst->dwDevType&0xff;
	// if( type!=DI8DEVTYPE_JOYSTICK&&type!=DI8DEVTYPE_GAMEPAD ) return DIENUM_CONTINUE;

	if( Joystick *joy=createJoystick( (DirectInput8Driver*)pvRef,devinst ) ){
		joysticks.push_back( joy );
	}
	return DIENUM_CONTINUE;
}

DirectInput8Driver::DirectInput8Driver( IDirectInput8 *di ):
dirInput(di){
	joysticks.clear();
	dirInput->EnumDevices( DI8DEVCLASS_GAMECTRL,enumJoystick,this,DIEDFL_ATTACHEDONLY );
}

DirectInput8Driver::~DirectInput8Driver(){
	for( int k=0;k<joysticks.size();++k ) delete joysticks[k];
	joysticks.clear();

	dirInput->Release();
}

void DirectInput8Driver::reset(){
	for( int k=0;k<joysticks.size();++k ) joysticks[k]->reset();
}

BBDevice *DirectInput8Driver::getJoystick( int n )const{
	return n>=0 && n<joysticks.size() ? joysticks[n] : 0;
}

int DirectInput8Driver::getJoystickType( int n )const{
	return n>=0 && n<joysticks.size() ? joysticks[n]->type : 0;
}

int DirectInput8Driver::numJoysticks()const{
	return joysticks.size();
}

int DirectInput8Driver::toAscii( int scan )const{
	switch( scan ){
	case DIK_INSERT:return ASC_INSERT;
	case DIK_DELETE:return ASC_DELETE;
	case DIK_HOME:return ASC_HOME;
	case DIK_END:return ASC_END;
	case DIK_PGUP:return ASC_PAGEUP;
	case DIK_PGDN:return ASC_PAGEDOWN;
	case DIK_UP:return ASC_UP;
	case DIK_DOWN:return ASC_DOWN;
	case DIK_LEFT:return ASC_LEFT;
	case DIK_RIGHT:return ASC_RIGHT;
	}
	scan&=0x7f;
 	int virt=MapVirtualKey( scan,1 );
	if( !virt ) return 0;

	// TODO: figure out a better way than calling bbKeyDown
	static unsigned char mat[256];
	mat[VK_LSHIFT]=bbKeyDown( DIK_LSHIFT ) ? 0x80 : 0;
	mat[VK_RSHIFT]=bbKeyDown( DIK_RSHIFT ) ? 0x80 : 0;
	mat[VK_SHIFT]=mat[VK_LSHIFT]|mat[VK_RSHIFT];
	mat[VK_LCONTROL]=bbKeyDown( DIK_LCONTROL ) ? 0x80 : 0;
	mat[VK_RCONTROL]=bbKeyDown( DIK_RCONTROL ) ? 0x80 : 0;
	mat[VK_CONTROL]=mat[VK_LCONTROL]|mat[VK_RCONTROL];
	mat[VK_LMENU]=bbKeyDown( DIK_LMENU ) ? 0x80 : 0;
	mat[VK_RMENU]=bbKeyDown( DIK_RMENU ) ? 0x80 : 0;
	mat[VK_MENU]=mat[VK_LMENU]|mat[VK_RMENU];

	WORD ch;
	if( ToAscii( virt,scan,mat,&ch,0 )!=1 ) return 0;
	return ch & 255;
}

void BBCALL bbEnableDirectInput( int enable ){
}

int  BBCALL bbDirectInputEnabled(){
	return false;
}

BBMODULE_CREATE( input_directinput8 ){
	if( !gx_input ){
		IDirectInput8 *di;
		if( DirectInput8Create( GetModuleHandle(0),DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&di,0 )>=0 ){
			gx_input=d_new DirectInput8Driver( di );
			if( !bbEnumInput() ){
				_bbDebugInfo( "Failed to enumerate input devices" );
				gx_input=0;
			} else {
				bbSystemProperties["directinput8"]=itoa( (bb_int_t)di );
			}
		}else{
			_bbDebugInfo( "Create DirectInput failed" );
		}
	}
	return true;
}

BBMODULE_DESTROY( input_directinput8 ){
	return true;
}
