
#include <cstring>
#include "device.h"

BBDevice::BBDevice(){
	memset( name,0,sizeof(name) );
	reset();
}

BBDevice::~BBDevice(){
}

void BBDevice::reset(){
	memset( down_state,0,sizeof(down_state) );
	memset( axis_states,0,sizeof(axis_states) );
	memset( hit_count,0,sizeof(hit_count) );
	put=get=0;
}

void BBDevice::downEvent( int key ){
	down_state[key]=true;
	++hit_count[key];
}

void BBDevice::upEvent( int key ){
	down_state[key]=false;
}

void BBDevice::charEvent( int code ){
	if( put-get<QUE_SIZE ) que[put++&QUE_MASK]=code;
}

void BBDevice::setDownState( int key,bool down ){
	if( down==down_state[key] ) return;
	if( down ) downEvent( key );
	else upEvent( key );
}

void BBDevice::flush(){
	update();
	memset( hit_count,0,sizeof(hit_count) );
	put=get=0;
}

bool BBDevice::keyDown( int key ){
	update();
	return down_state[key];
}

int BBDevice::keyHit( int key ){
	update();
	int n=hit_count[key];
	hit_count[key]-=n;
	return n;
}

int BBDevice::getKey(){
	update();
	return get<put ? que[get++ & QUE_MASK] : 0;
}

float BBDevice::getAxisState( int axis ){
	update();
	return axis_states[axis];
}

const char *BBDevice::getId(){
	return id;
}

const char *BBDevice::getName(){
	return name;
}
