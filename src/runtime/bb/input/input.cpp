
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

int BBCALL bbKeyDown( int n ){
	return bbKeyboard.keyDown( n );
}

int BBCALL bbKeyHit( int n ){
	return bbKeyboard.keyHit( n );
}

int BBCALL bbGetKey( int ascii ){
	int key=bbKeyboard.getKey();
	return ascii ? gx_input->toAscii( key ) : key;
}

int BBCALL bbWaitKey(){
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

int BBCALL bbMouseDown( int n ){
	return bbMouse.keyDown( n );
}

int BBCALL bbMouseHit( int n ){
	return bbMouse.keyHit( n );
}

int BBCALL bbGetMouse(){
	return bbMouse.getKey();
}

int BBCALL bbWaitMouse(){
	for(;;){
		if( !bbRuntimeIdle()) RTEX( 0 );
		if( int key=bbMouse.getKey() ) return key;
		bbDelay( 20 );
	}
}

int BBCALL bbMouseWait(){
	return bbWaitMouse();
}

int BBCALL bbMouseX(){
	return bbMouse.getAxisState( 0 );
}

int BBCALL bbMouseY(){
	return bbMouse.getAxisState( 1 );
}

int BBCALL bbMouseZ(){
	return bbMouse.getAxisState( 2 )/120;
}

int BBCALL bbMouseXSpeed(){
	int dx=bbMouseX()-mouse_x;
	mouse_x+=dx;
	return dx;
}

int BBCALL bbMouseYSpeed(){
	int dy=bbMouseY()-mouse_y;
	mouse_y+=dy;
	return dy;
}

int BBCALL bbMouseZSpeed(){
	int dz=bbMouseZ()-mouse_z;
	mouse_z+=dz;
	return dz;
}

void BBCALL bbFlushMouse(){
	bbMouse.flush();
}

int BBCALL bbCountJoys(){
	return bbJoysticks.size();
}

int BBCALL bbFindJoy( BBStr *id ){
	string i=*id;delete id;
	for( int k=0;k<bbJoysticks.size();++k ){
		if( strcmp( i.c_str(),bbJoysticks[k]->getId() )==0 ) return k;
	}
	return -1;
}

BBStr * BBCALL bbJoyID( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return d_new BBStr( "" );
	return d_new BBStr( bbJoysticks[port]->getId() );
}

BBStr * BBCALL bbJoyName( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return d_new BBStr( "" );
	return d_new BBStr( bbJoysticks[port]->getName() );
}

int BBCALL bbJoyType( int port ){
	return gx_input->getJoystickType( port );
}

int BBCALL bbJoyDown( int n,int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->keyDown( n );
}

int BBCALL bbJoyHit( int n,int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->keyHit( n );
}

int BBCALL bbGetJoy( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getKey();
}

int BBCALL bbWaitJoy( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	for(;;){
		if( !bbRuntimeIdle() ) RTEX( 0 );
		if( int key=bbJoysticks[port]->getKey() ) return key;
		bbDelay( 20 );
	}
}

float BBCALL bbJoyX( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(0);
}

float BBCALL bbJoyY( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(1);
}

float BBCALL bbJoyZ( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(2);
}

float BBCALL bbJoyU( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(3);
}

float BBCALL bbJoyV( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(4);
}

float BBCALL bbJoyPitch( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(5)*180;
}

float BBCALL bbJoyYaw( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(6)*180;
}

float BBCALL bbJoyRoll( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(7)*180;
}

int BBCALL bbJoyHat( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	return bbJoysticks[port]->getAxisState(8);
}

int BBCALL bbJoyXDir( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	float t=bbJoysticks[port]->getAxisState(0);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

int BBCALL bbJoyYDir( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	float t=bbJoysticks[port]->getAxisState(1);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

int	BBCALL bbJoyZDir( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	float t=bbJoysticks[port]->getAxisState(2);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

int	BBCALL bbJoyUDir( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	float t=bbJoysticks[port]->getAxisState(3);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

int	BBCALL bbJoyVDir( int port ){
	if( port<0 || port>=bbJoysticks.size() ) return 0;
	float t=bbJoysticks[port]->getAxisState(4);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

void BBCALL bbFlushJoy(){
	for( int k=0;k<bbJoysticks.size();++k ) bbJoysticks[k]->flush();
}

}
