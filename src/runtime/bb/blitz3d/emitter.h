
#ifndef EMITTER_H
#define EMITTER_H

#include "object.h"

class Emitter : public Object{
public:
	Emitter();
	Emitter( const Emitter &t );
	~Emitter();

	//Entity interface
	Entity *clone(){ return d_new Emitter( *this ); }
	Emitter *getEmitter(){ return this; }

	//Object interface
	void beginRender( float tween );

	//Public interface
	BBChannel *emitSound( BBSound *sound );

private:
	Vector pos,vel;

	vector<BBChannel*> channels;
};

#endif
