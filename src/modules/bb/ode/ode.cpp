
#include "ode.h"
#include <ode/ode.h>
#include <bb/blitz3d/blitz3d.h>

float _odeVector[4];

extern dSurfaceParameters _odeSurfaceParams;

BBLIB void BBCALL _odeInitODE(){
  dInitODE();
}

BBLIB void BBCALL _odeCloseODE(){
  dCloseODE();
}

BBLIB float BBCALL _odeVectorX(){
  return _odeVector[0];
}

BBLIB float BBCALL _odeVectorY(){
  return _odeVector[1];
}

BBLIB float BBCALL _odeVectorZ(){
  return _odeVector[2];
}

BBLIB float BBCALL _odeVectorW(){
  return _odeVector[3];
}

BBLIB void BBCALL _odeGeomSyncEntity( bb_ptr_t g,Entity *e ){
	dQuaternion dq;
	dGeomGetQuaternion( (dGeomID)g,dq );
	Quat q;
	q.v.x=dq[0];
	q.v.y=dq[1];
	q.v.z=dq[2];
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
