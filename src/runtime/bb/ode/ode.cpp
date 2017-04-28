
#include "ode.h"
#include <ode/ode.h>

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

BBMODULE_CREATE( ode ){
  memset( &_odeSurfaceParams,0,sizeof(dSurfaceParameters) );
  return true;
}

BBMODULE_DESTROY( ode ){
  return true;
}
