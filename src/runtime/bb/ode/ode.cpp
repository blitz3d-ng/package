
#include "ode.h"
#include <ode/ode.h>
#include <bb/blitz3d/blitz3d.h>

float _odeVector[4];

extern dSurfaceParameters _odeSurfaceParams;

void BBCALL _odeInitODE(){
  dInitODE();
}

void BBCALL _odeCloseODE(){
  dCloseODE();
}

float BBCALL _odeVectorX(){
  return _odeVector[0];
}

float BBCALL _odeVectorY(){
  return _odeVector[1];
}

float BBCALL _odeVectorZ(){
  return _odeVector[2];
}

float BBCALL _odeVectorW(){
  return _odeVector[3];
}

void BBCALL _odeGeomSyncEntity( bb_ptr_t g,Entity *e ){
	dQuaternion dq;
	dGeomGetQuaternion( (dGeomID)g,dq );
	Quat q;
	q.v.x=dq[2];
	q.v.y=dq[1];
	q.v.z=dq[0];
	q.w=dq[3];
	e->setWorldPosition( Vector(dGeomGetPosition((dGeomID)g)) );
	e->setWorldRotation( q );
}

BBMODULE_CREATE( ode ){
  memset( &_odeSurfaceParams,0,sizeof(dSurfaceParameters) );
  return true;
}

BBMODULE_DESTROY( ode ){
  return true;
}
