
#include "std.h"
#include "light.h"
#include "scene.h"

extern BBScene *bbScene;

Light::Light( int type ):
type(type),range(1000),color(1.0f,1.0f,1.0f),innerAngle(0.0f),outerAngle(0.0f){
	rep=bbScene->createLight( type );
}

Light::~Light(){
	bbScene->freeLight( rep );
	rep->markDirty();
}

float Light::getRange(){
	return range;
}

void Light::setRange( float r ){
	range=r;
	rep->markDirty();
}

const Vector &Light::getColor(){
	return color;
}

void Light::setColor( const Vector &v ){
	color=v;
	rep->markDirty();
}

void Light::getConeAngles( float *inner,float *outer ){
	*inner=innerAngle;
	*outer=outerAngle;
}

void Light::setConeAngles( float inner,float outer ){
	innerAngle=inner;
	outerAngle=outer;
	rep->markDirty();
}

bool Light::beginRender( float tween ){
	Object::beginRender( tween );
	rep->update( this );
	return true;
}
