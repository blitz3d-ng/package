
#include "../../stdutil/stdutil.h"
#include <bb/blitz/blitz.h>
#include <bb/runtime/runtime.h>
#include <bb/system/system.h>
#include "input.h"

#include <vector>
#include <string.h>
using namespace std;

BBInputDriver *gx_input;
BBDevice *gx_mouse;
BBDevice *gx_keyboard;
vector<BBDevice*> gx_joysticks;

static int mouse_x,mouse_y,mouse_z;
static const float JLT=-1.0f/3.0f;
static const float JHT=1.0f/3.0f;

int BBCALL bbEnumInput(){
	if( gx_input ){
		if( gx_keyboard=gx_input->getKeyboard() ){
			if( gx_mouse=gx_input->getMouse() ){
				gx_joysticks.clear();
				for( int k=0;k<gx_input->numJoysticks();++k ){
					gx_joysticks.push_back( gx_input->getJoystick(k) );
				}
				mouse_x=mouse_y=mouse_z=0;
				return true;
			}
		}
		return true;
	}
	return false;
}

BBMODULE_CREATE( input ){
	gx_input=0;
	return true;
}

BBMODULE_DESTROY( input ){
	gx_joysticks.clear();
	if( gx_input ){
		delete gx_input;
		gx_input=0;
	}
	return true;
}

int BBCALL bbKeyDown( int n ){
	return gx_keyboard->keyDown( n );
}

int BBCALL bbKeyHit( int n ){
	return gx_keyboard->keyHit( n );
}

int BBCALL bbGetKey(){
	return gx_input->toAscii( gx_keyboard->getKey() );
}

int BBCALL bbWaitKey(){
	for(;;){
		if( !bbRuntimeIdle() ) RTEX( 0 );
		if( int key=gx_keyboard->getKey( ) ){
			if( key=gx_input->toAscii( key ) ) return key;
		}
		bbDelay( 20 );
	}
}

void BBCALL bbFlushKeys(){
	gx_keyboard->flush();
}

int BBCALL bbMouseDown( int n ){
	return gx_mouse->keyDown( n );
}

int BBCALL bbMouseHit( int n ){
	return gx_mouse->keyHit( n );
}

int BBCALL bbGetMouse(){
	return gx_mouse->getKey();
}

int BBCALL bbWaitMouse(){
	for(;;){
		if( !bbRuntimeIdle()) RTEX( 0 );
		if( int key=gx_mouse->getKey() ) return key;
		bbDelay( 20 );
	}
}

int BBCALL bbMouseWait(){
	return bbWaitMouse();
}

int BBCALL bbMouseX(){
	return gx_mouse->getAxisState( 0 );
}

int BBCALL bbMouseY(){
	return gx_mouse->getAxisState( 1 );
}

int BBCALL bbMouseZ(){
	return gx_mouse->getAxisState( 2 )/120;
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
	gx_mouse->flush();
}

void BBCALL bbMoveMouse( int x,int y ){
	gx_input->moveMouse( mouse_x=x,mouse_y=y );
}

int BBCALL bbCountJoys(){
	return gx_joysticks.size();
}

int BBCALL bbFindJoy( BBStr *id ){
	string i=*id;delete id;
	for( int k=0;k<gx_joysticks.size();++k ){
		if( strcmp( i.c_str(),gx_joysticks[k]->getId() )==0 ) return k;
	}
	return -1;
}

BBStr * BBCALL bbJoyID( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return d_new BBStr( "" );
	return d_new BBStr( gx_joysticks[port]->getId() );
}

BBStr * BBCALL bbJoyName( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return d_new BBStr( "" );
	return d_new BBStr( gx_joysticks[port]->getName() );
}

int BBCALL bbJoyType( int port ){
	return gx_input->getJoystickType( port );
}

int BBCALL bbJoyDown( int n,int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->keyDown( n );
}

int BBCALL bbJoyHit( int n,int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->keyHit( n );
}

int BBCALL bbGetJoy( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->getKey();
}

int BBCALL bbWaitJoy( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	for(;;){
		if( !bbRuntimeIdle() ) RTEX( 0 );
		if( int key=gx_joysticks[port]->getKey() ) return key;
		bbDelay( 20 );
	}
}

float BBCALL bbJoyX( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->getAxisState(0);
}

float BBCALL bbJoyY( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->getAxisState(1);
}

float BBCALL bbJoyZ( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->getAxisState(2);
}

float BBCALL bbJoyU( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->getAxisState(3);
}

float BBCALL bbJoyV( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->getAxisState(4);
}

float BBCALL bbJoyPitch( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->getAxisState(5)*180;
}

float BBCALL bbJoyYaw( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->getAxisState(6)*180;
}

float BBCALL bbJoyRoll( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->getAxisState(7)*180;
}

int BBCALL bbJoyHat( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	return gx_joysticks[port]->getAxisState(8);
}

int BBCALL bbJoyXDir( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	float t=gx_joysticks[port]->getAxisState(0);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

int BBCALL bbJoyYDir( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	float t=gx_joysticks[port]->getAxisState(1);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

int	BBCALL bbJoyZDir( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	float t=gx_joysticks[port]->getAxisState(2);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

int	BBCALL bbJoyUDir( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	float t=gx_joysticks[port]->getAxisState(3);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

int	BBCALL bbJoyVDir( int port ){
	if( port<0 || port>=gx_joysticks.size() ) return 0;
	float t=gx_joysticks[port]->getAxisState(4);
	return t<JLT ? -1 : ( t>JHT ? 1 : 0 );
}

void BBCALL bbFlushJoy(){
	for( int k=0;k<gx_joysticks.size();++k ) gx_joysticks[k]->flush();
}
