
#include "driver.h"
#include "../../gxruntime/gxruntime.h"

#include <dinput.h>

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

class Keyboard : public Device{
public:
	Keyboard( DirectInput8Driver *i,IDirectInputDevice8 *d ):Device(i,d){
	}
	void update(){
		if( !acquired ){
			input->runtime->idle();
			return;
		}
		int k,cnt=32;
		DIDEVICEOBJECTDATA data[32],*curr;
		if( device->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),data,(DWORD*)&cnt,0 )<0 ) return;
		curr=data;
		for( k=0;k<cnt;++curr,++k ){
			int n=curr->dwOfs;if( !n || n>255 ) continue;
			if( curr->dwData&0x80 ) downEvent( n );
			else upEvent( n );
		}
	}
};

class Mouse : public Device{
public:
	Mouse( DirectInput8Driver *i,IDirectInputDevice8 *d ):Device(i,d){
	}
	void update(){
		if( !acquired ){
			input->runtime->idle();
			return;
		}
		DIMOUSESTATE state;
		if( device->GetDeviceState(sizeof(state),&state)<0 ) return;
		if( gxGraphics *g=input->runtime->graphics ){
			int mx=axis_states[0]+state.lX;
			int my=axis_states[1]+state.lY;
			if( mx<0 ) mx=0;
			else if( mx>=g->getWidth() ) mx=g->getWidth()-1;
			if( my<0 ) my=0;
			else if( my>=g->getHeight() ) my=g->getHeight()-1;
			axis_states[0]=mx;
			axis_states[1]=my;
			axis_states[2]+=state.lZ;
		}
		for( int k=0;k<3;++k ){
			setDownState( k+1,state.rgbButtons[k]&0x80 ? true : false );
		}
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
			input->runtime->idle();
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

static Keyboard *keyboard;
static Mouse *mouse;
static vector<Joystick*> joysticks;

static Keyboard *createKeyboard( DirectInput8Driver *input ){
	IDirectInputDevice8 *dev;
	if( input->dirInput->CreateDevice( GUID_SysKeyboard,&dev,0 )>=0 ){
		if( dev->SetCooperativeLevel( input->runtime->hwnd,DISCL_FOREGROUND|DISCL_EXCLUSIVE )>=0 ){

			if( dev->SetDataFormat( &c_dfDIKeyboard )>=0 ){
				DIPROPDWORD dword;
	 			memset( &dword,0,sizeof(dword) );
				dword.diph.dwSize=sizeof(DIPROPDWORD);
				dword.diph.dwHeaderSize=sizeof(DIPROPHEADER);
				dword.diph.dwObj=0;
				dword.diph.dwHow=DIPH_DEVICE;
				dword.dwData=32;
				if( dev->SetProperty( DIPROP_BUFFERSIZE,&dword.diph )>=0 ){
					return d_new Keyboard( input,dev );
				}else{
//					input->runtime->debugInfo( "keyboard: SetProperty failed" );
				}
			}else{
//				input->runtime->debugInfo( "keyboard: SetDataFormat failed" );
			}
			return d_new Keyboard( input,dev );

		}else{
			input->runtime->debugInfo( "keyboard: SetCooperativeLevel failed" );
		}
		dev->Release();
	}else{
		input->runtime->debugInfo( "keyboard: CreateDeviceEx failed" );
	}
	return 0;
}

static Mouse *createMouse( DirectInput8Driver *input ){
	IDirectInputDevice8 *dev;
	if( input->dirInput->CreateDevice( GUID_SysMouse,&dev,0 )>=0 ){
		if( dev->SetCooperativeLevel( input->runtime->hwnd,DISCL_FOREGROUND|DISCL_EXCLUSIVE )>=0 ){

			if( dev->SetDataFormat( &c_dfDIMouse )>=0 ){
				return d_new Mouse( input,dev );
			}else{
//				input->runtime->debugInfo( "mouse: SetDataFormat failed" );
			}
			return d_new Mouse( input,dev );

		}else{
			input->runtime->debugInfo( "mouse: SetCooperativeLevel failed" );
		}
		dev->Release();
	}else{
		input->runtime->debugInfo( "mouse: CreateDeviceEx failed" );
	}
	return 0;
}

static Joystick *createJoystick( DirectInput8Driver *input,LPCDIDEVICEINSTANCE devinst ){
	IDirectInputDevice8 *dev;
	if( input->dirInput->CreateDevice( devinst->guidInstance,&dev,0 )>=0 ){
		if( dev->SetCooperativeLevel( input->runtime->hwnd,DISCL_FOREGROUND|DISCL_EXCLUSIVE )>=0 ){
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
	// if( (devinst->dwDevType&0xff)!=DI8DEVCLASS_GAMECTRL ) return DIENUM_CONTINUE;

	if( Joystick *joy=createJoystick( (DirectInput8Driver*)pvRef,devinst ) ){
		joysticks.push_back( joy );
	}
	return DIENUM_CONTINUE;
}

DirectInput8Driver::DirectInput8Driver( gxRuntime *rt,IDirectInput8 *di ):
runtime(rt),dirInput(di){
	keyboard=createKeyboard( this );
	mouse=createMouse( this );
	joysticks.clear();
	dirInput->EnumDevices( DI8DEVCLASS_GAMECTRL,enumJoystick,this,DIEDFL_ATTACHEDONLY );
}

DirectInput8Driver::~DirectInput8Driver(){
	for( int k=0;k<joysticks.size();++k ) delete joysticks[k];
	joysticks.clear();
	delete mouse;
	delete keyboard;

	dirInput->Release();
}

void DirectInput8Driver::wm_keydown( int key ){
	if( keyboard ) keyboard->downEvent( key );
}

void DirectInput8Driver::wm_keyup( int key ){
	if( keyboard ) keyboard->upEvent( key );
}

void DirectInput8Driver::wm_mousedown( int key ){
	if( mouse ) mouse->downEvent( key );
}

void DirectInput8Driver::wm_mouseup( int key ){
	if( mouse ) mouse->upEvent( key );
}

void DirectInput8Driver::wm_mousemove( int x,int y ){
	if( mouse ){
		mouse->axis_states[0]=x;
		mouse->axis_states[1]=y;
	}
}

void DirectInput8Driver::wm_mousewheel( int dz ){
	if( mouse ) mouse->axis_states[2]+=dz;
}

void DirectInput8Driver::reset(){
	if( mouse ) mouse->reset();
	if( keyboard ) keyboard->reset();
	for( int k=0;k<joysticks.size();++k ) joysticks[k]->reset();
}

bool DirectInput8Driver::acquire(){
	bool m_ok=true,k_ok=true;
	if( mouse ) m_ok=mouse->acquire();
	if( keyboard ) k_ok=keyboard->acquire();
	if( m_ok && k_ok ) return true;
	if( k_ok ) keyboard->unacquire();
	if( m_ok ) mouse->unacquire();
	return false;
}

void DirectInput8Driver::unacquire(){
	if( keyboard ) keyboard->unacquire();
	if( mouse ) mouse->unacquire();
}

void DirectInput8Driver::moveMouse( int x,int y ){
	if( !mouse ) return;
	mouse->axis_states[0]=x;
	mouse->axis_states[1]=y;
	runtime->moveMouse( x,y );
}

BBDevice *DirectInput8Driver::getMouse()const{
	return mouse;
}

BBDevice *DirectInput8Driver::getKeyboard()const{
	return keyboard;
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

	static unsigned char mat[256];
	mat[VK_LSHIFT]=keyboard->keyDown( DIK_LSHIFT ) ? 0x80 : 0;
	mat[VK_RSHIFT]=keyboard->keyDown( DIK_RSHIFT ) ? 0x80 : 0;
	mat[VK_SHIFT]=mat[VK_LSHIFT]|mat[VK_RSHIFT];
	mat[VK_LCONTROL]=keyboard->keyDown( DIK_LCONTROL ) ? 0x80 : 0;
	mat[VK_RCONTROL]=keyboard->keyDown( DIK_RCONTROL ) ? 0x80 : 0;
	mat[VK_CONTROL]=mat[VK_LCONTROL]|mat[VK_RCONTROL];
	mat[VK_LMENU]=keyboard->keyDown( DIK_LMENU ) ? 0x80 : 0;
	mat[VK_RMENU]=keyboard->keyDown( DIK_RMENU ) ? 0x80 : 0;
	mat[VK_MENU]=mat[VK_LMENU]|mat[VK_RMENU];

	WORD ch;
	if( ToAscii( virt,scan,mat,&ch,0 )!=1 ) return 0;
	return ch & 255;
}
