
#include "../../stdutil/stdutil.h"
#include <bb/blitz/blitz.h>
#include <bb/runtime/runtime.h>
#include <bb/system/system.h>
#include <bb/event/event.h>
#include "input.h"

#include <vector>
#include <string.h>
using namespace std;

BBInputDriver *gx_input;
BBDevice bbMouse;
BBDevice bbKeyboard;
vector<BBDevice*> bbJoysticks;

static int mouse_x,mouse_y,mouse_z;
static const float JLT=-1.0f/3.0f;
static const float JHT=1.0f/3.0f;

int BBCALL bbEnumInput(){
	if( gx_input ){
		bbJoysticks.clear();
		for( int k=0;k<gx_input->numJoysticks();++k ){
			bbJoysticks.push_back( gx_input->getJoystick(k) );
		}
		mouse_x=mouse_y=mouse_z=0;
		return true;
	}
	return false;
}

void processEvent( void *data,void *context){
	BBEvent *ev=(BBEvent*)data;
	switch( ev->id ){
	case BBEVENT_KEYDOWN:   bbKeyboard.downEvent( ev->data );break;
	case BBEVENT_KEYUP:     bbKeyboard.upEvent( ev->data );break;
	case BBEVENT_MOUSEDOWN: bbMouse.downEvent( ev->data );break;
	case BBEVENT_MOUSEUP:   bbMouse.upEvent( ev->data );break;
	case BBEVENT_MOUSEMOVE:
		if( ev->data>0 ){
			mouse_x=ev->x;
			mouse_y=ev->y;
		}
		bbMouse.axis_states[0]=ev->x;
		bbMouse.axis_states[1]=ev->y;
		break;
	case BBEVENT_MOUSEWHEEL:
		bbMouse.axis_states[2]+=ev->data;
		break;
	}
}

BBMODULE_CREATE( input ){
	gx_input=0;
	bbOnEvent.add( processEvent,0 );
	return true;
}

BBMODULE_DESTROY( input ){
	bbJoysticks.clear();
	if( gx_input ){
		delete gx_input;
		gx_input=0;
	}
	return true;
}

extern "C" {

bb_int_t BBCALL bbKeyDown( bb_int_t n ){
	return bbKeyboard.keyDown( n );
}

bb_int_t BBCALL bbKeyHit( bb_int_t n ){
	return bbKeyboard.keyHit( n );
}

bb_int_t BBCALL bbGetKey( bb_int_t ascii ){
	int key=bbKeyboard.getKey();
	return ascii ? gx_input->toAscii( key ) : key;
}

bb_int_t BBCALL bbWaitKey(){
	for(;;){
		if( !bbRuntimeIdle() ) RTEX( 0 );
		if( int key=bbKeyboard.getKey( ) ){
			if( (key=gx_input->toAscii( key )) ) return key;
		}
		bbDelay( 20 );
	}
}

void BBCALL bbFlushKeys(){
	bbKeyboard.flush();
}

bb_int_t BBCALL bbMouseDown( bb_int_t n ){
	return bbMouse.keyDown( n );
}

bb_int_t BBCALL bbMouseHit( bb_int_t n ){
	return bbMouse.keyHit( n );
}

bb_int_t BBCALL bbGetMouse(){
	return bbMouse.getKey();
}

bb_int_t BBCALL bbWaitMouse(){
	for(;;){
		if( !bbRuntimeIdle()) RTEX( 0 );
		if( int key=bbMouse.getKey() ) return key;
		bbDelay( 20 );
	}
}

bb_int_t BBCALL bbMouseWait(){
	return bbWaitMouse();
}

bb_int_t BBCALL bbMouseX(){
	return bbMouse.getAxisState( 0 );
}

bb_int_t BBCALL bbMouseY(){
	return bbMouse.getAxisState( 1 );
}

bb_int_t BBCALL bbMouseZ(){
	return bbMouse.getAxisState( 2 )/120;
}

bb_int_t BBCALL bbMouseXSpeed(){
	int dx=bbMouseX()-mouse_x;
	mouse_x+=dx;
	return dx;
}

bb_int_t BBCALL bbMouseYSpeed(){
	int dy=bbMouseY()-mouse_y;
	mouse_y+=dy;
	return dy;
}

bb_int_t BBCALL bbMouseZSpeed(){
	int dz=bbMouseZ()-mouse_z;
	mouse_z+=dz;
	return dz;
}

void BBCALL bbFlushMouse(){
	bbMouse.flush();
}

bb_int_t BBCALL bbCountJoys(){
	return bbJoysticks.size();
}

bb_int_t BBCALL bbFindJoy( BBStr *id ){
	string i=*id;delete id;
	for( int k=0;k<bbJoysticks.size();++k ){
		if( strcmp( i.c_str(),bbJoysticks[k]->getId() )==0 ) return k;
	}
	return -1;
}

BBStr * BBCALL bbJoyID( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return d_new BBStr( "" );
	return d_new BBStr( bbJoysticks[port]->getId() );
}

BBStr * BBCALL bbJoyName( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return d_new BBStr( "" );
	return d_new BBStr( bbJoysticks[port]->getName() );
}

bb_int_t BBCALL bbJoyType( bb_int_t port ){
	return gx_input->getJoystickType( port );
}

bb_int_t BBCALL bbJoyDown( bb_int_t n,bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->keyDown( n );
}

bb_int_t BBCALL bbJoyHit( bb_int_t n,bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->keyHit( n );
}

bb_int_t BBCALL bbGetJoy( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getKey();
}

bb_int_t BBCALL bbWaitJoy( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	for(;;){
		if( !bbRuntimeIdle() ) RTEX( 0 );
		if( int key=bbJoysticks[port]->getKey() ) return key;
		bbDelay( 20 );
	}
}

bb_float_t BBCALL bbJoyX( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(0);
}

bb_float_t BBCALL bbJoyY( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(1);
}

bb_float_t BBCALL bbJoyZ( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(2);
}

bb_float_t BBCALL bbJoyU( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(3);
}

bb_float_t BBCALL bbJoyV( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(4);
}

bb_float_t BBCALL bbJoyPitch( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(5)*180;
}

bb_float_t BBCALL bbJoyYaw( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(6)*180;
}

bb_float_t BBCALL bbJoyRoll( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(7)*180;
}

bb_int_t BBCALL bbJoyHat( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(8);
}

bb_int_t BBCALL bbJoyXDir( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	float t=bbJoysticks[port]->getAxisState(0);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

bb_int_t BBCALL bbJoyYDir( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	float t=bbJoysticks[port]->getAxisState(1);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

bb_int_t	BBCALL bbJoyZDir( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	float t=bbJoysticks[port]->getAxisState(2);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

bb_int_t	BBCALL bbJoyUDir( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	float t=bbJoysticks[port]->getAxisState(3);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

bb_int_t	BBCALL bbJoyVDir( bb_int_t port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	float t=bbJoysticks[port]->getAxisState(4);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

void BBCALL bbFlushJoy(){
	for( int k=0;k<bbJoysticks.size();++k ) bbJoysticks[k]->flush();
}

}
