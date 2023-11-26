
#ifndef BBDEVICE_H
#define BBDEVICE_H

#include <bb/runtime/runtime.h>

class BBDevice{
public:
	float axis_states[32];

	BBDevice();
	virtual ~BBDevice();

	virtual void update(){
		bbRuntimeIdle();
	}

	void reset();
	void downEvent( int key,bool que=true );
	void upEvent( int key );
	void queue( int code );
	void setDownState( int key,bool down );

protected:
	enum{
		QUE_SIZE=32,QUE_MASK=QUE_SIZE-1
	};
	int hit_count[256];			//how many hits of key
	bool down_state[256];			//time key went down
	int que[QUE_SIZE],put,get;

protected:
	char id[64],name[64];

	/***** GX INTERFACE *****/
public:
	void flush();

	bool keyDown( int key );

	int keyHit( int key );

	int getKey();

	float getAxisState( int axis );

	const char *getId();
	const char *getName();
};

#endif
