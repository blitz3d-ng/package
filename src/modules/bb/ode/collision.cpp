
#include "ode.h"
#include <ode/ode.h>

#define MAX_CONTACTS 256

int _odeMaxContacts=MAX_CONTACTS;
dSurfaceParameters _odeSurfaceParams;
float _odeFDir[3]={ 0.0f,0.0f,0.0f };

struct CollideEnv{
  dWorldID world;
  dJointGroupID group;
};

static void near_callback( void *data,dGeomID o1,dGeomID o2 ){
  CollideEnv &env=*(CollideEnv*)data;

  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);

  dContact contact[MAX_CONTACTS];

  for( int i=0;i<_odeMaxContacts;i++ ){
    contact[i].surface=_odeSurfaceParams;
    contact[i].fdir1[0]=_odeFDir[0];
    contact[i].fdir1[1]=_odeFDir[1];
    contact[i].fdir1[2]=_odeFDir[2];
  }

  if (int numc = dCollide( o1,o2,_odeMaxContacts,&contact[0].geom,sizeof(dContact) )) {
    for( int i = 0;i<numc;i++ ){
      dJointID c = dJointCreateContact( env.world,env.group,contact+i );
      dJointAttach(c, b1, b2);
    }
  }
}

BBLIB void BBCALL _odeSpaceCollide( bb_ptr_t s,bb_ptr_t w,bb_ptr_t g ){
  CollideEnv env;
  env.world=(dWorldID)w;
  env.group=(dJointGroupID)g;

  dSpaceCollide( (dSpaceID)s,&env,near_callback );
}

BBLIB void BBCALL _odeContactSetMaxContacts( bb_int_t max_contacts ){
  _odeMaxContacts=max_contacts < MAX_CONTACTS ? max_contacts : MAX_CONTACTS;
}

BBLIB bb_int_t BBCALL _odeContactGetMaxContacts(){
  return _odeMaxContacts;
}

BBLIB void BBCALL _odeContactSetMode( bb_int_t mode ){
  _odeSurfaceParams.mode=mode;
}

BBLIB bb_int_t BBCALL _odeContactGetMode(){
  return _odeSurfaceParams.mode;
}

BBLIB void BBCALL _odeContactSetMu( bb_float_t mu ){
  _odeSurfaceParams.mu=mu;
}

BBLIB bb_float_t BBCALL _odeContactGetMu(){
  return _odeSurfaceParams.mu;
}

BBLIB void BBCALL _odeContactSetMu2( bb_float_t mu ){
  _odeSurfaceParams.mu2=mu;
}

BBLIB bb_float_t BBCALL _odeContactGetMu2(){
  return _odeSurfaceParams.mu2;
}

BBLIB void BBCALL _odeContactSetFDir1( bb_float_t fdir1x,bb_float_t fdir1y,bb_float_t fdir1z ){
  _odeFDir[0]=fdir1x;
  _odeFDir[1]=fdir1y;
  _odeFDir[2]=fdir1z;
}

BBLIB void BBCALL _odeContactGetFDir1(){
  _odeVector[0]=_odeFDir[0];
  _odeVector[1]=_odeFDir[1];
  _odeVector[2]=_odeFDir[2];
}

BBLIB void BBCALL _odeContactSetBounce( bb_float_t bounce ){
  _odeSurfaceParams.bounce=bounce;
}

BBLIB bb_float_t BBCALL _odeContactGetBounce(){
  return _odeSurfaceParams.bounce;
}

BBLIB void BBCALL _odeContactSetBounceVel( bb_float_t vel ){
  _odeSurfaceParams.bounce_vel=vel;
}

BBLIB bb_float_t BBCALL _odeContactGetBounceVel(){
  return _odeSurfaceParams.bounce_vel;
}

BBLIB void BBCALL _odeContactSetSoftERP( bb_float_t erp ){
  _odeSurfaceParams.soft_erp=erp;
}

BBLIB bb_float_t BBCALL _odeContactGetSoftERP(){
  return _odeSurfaceParams.soft_erp;
}

BBLIB void BBCALL _odeContactSetSoftCFM( bb_float_t cfm ){
  _odeSurfaceParams.soft_cfm=cfm;
}

BBLIB bb_float_t BBCALL _odeContactGetSoftCFM(){
  return _odeSurfaceParams.soft_cfm;
}

BBLIB void BBCALL _odeContactSetMotion1( bb_float_t motion ){
  _odeSurfaceParams.motion1=motion;
}

BBLIB bb_float_t BBCALL _odeContactGetMotion1(){
  return _odeSurfaceParams.motion1;
}

BBLIB void BBCALL _odeContactSetMotion2( bb_float_t motion ){
  _odeSurfaceParams.motion2=motion;
}

BBLIB bb_float_t BBCALL _odeContactGetMotion2(){
  return _odeSurfaceParams.slip2;
}

BBLIB void BBCALL _odeContactSetSlip1( bb_float_t slip ){
  _odeSurfaceParams.slip1=slip;
}

BBLIB bb_float_t BBCALL _odeContactGetSlip1(){
  return _odeSurfaceParams.slip1;
}

BBLIB void BBCALL _odeContactSetSlip2( bb_float_t slip ){
  _odeSurfaceParams.slip2=slip;
}

BBLIB bb_float_t BBCALL _odeContactGetSlip2(){
  return _odeSurfaceParams.slip2;
}
