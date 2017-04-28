
#include "ode.h"
#include <ode/ode.h>
#include <bb/blitz3d/geom.h>

//degrees to radians and back
static const float dtor=0.0174532925199432957692369076848861f;
static const float rtod=1/dtor;

// ODE_API dReal dBodyGetAutoDisableLinearThreshold (dBodyID);
// ODE_API void  dBodySetAutoDisableLinearThreshold (dBodyID, dReal linear_average_threshold);
// ODE_API dReal dBodyGetAutoDisableAngularThreshold (dBodyID);
// ODE_API void  dBodySetAutoDisableAngularThreshold (dBodyID, dReal angular_average_threshold);
// ODE_API int dBodyGetAutoDisableAverageSamplesCount (dBodyID);
// ODE_API void dBodySetAutoDisableAverageSamplesCount (dBodyID, unsigned int average_samples_count);
// ODE_API int dBodyGetAutoDisableSteps (dBodyID);
// ODE_API void dBodySetAutoDisableSteps (dBodyID, int steps);
// ODE_API dReal dBodyGetAutoDisableTime (dBodyID);
// ODE_API void  dBodySetAutoDisableTime (dBodyID, dReal time);
// ODE_API int dBodyGetAutoDisableFlag (dBodyID);

void BBCALL _odeBodySetAutoDisableFlag( bb_ptr_t b,int do_auto_disable ){
  dBodySetAutoDisableFlag( (dBodyID)b,do_auto_disable );
}

// ODE_API void  dBodySetAutoDisableDefaults (dBodyID);
// ODE_API dWorldID dBodyGetWorld (dBodyID);

bb_ptr_t BBCALL _odeBodyCreate( bb_ptr_t w ){
  return (bb_ptr_t)dBodyCreate( (dWorldID)w );
}

// ODE_API void dBodyDestroy (dBodyID);
// ODE_API void  dBodySetData (dBodyID, void *data);
// ODE_API void *dBodyGetData (dBodyID);

void BBCALL _odeBodySetPosition( bb_ptr_t b,float x,float y,float z ){
  dBodySetPosition( (dBodyID)b,x,y,-z );
}

void BBCALL _odeBodySetRotation( bb_ptr_t b,float p,float y,float r ){
  Matrix mat=rotationMatrix( p*dtor,y*dtor,r*dtor );

  dMatrix3 m;
  m[ 0]=mat.i.x;
  m[ 1]=mat.i.y;
  m[ 2]=mat.i.z;
  m[ 3]=0.0f;
  m[ 4]=mat.j.x;
  m[ 5]=mat.j.y;
  m[ 6]=mat.j.z;
  m[ 7]=0.0f;
  m[ 8]=mat.k.x;
  m[ 9]=mat.k.y;
  m[10]=mat.k.z;
  m[11]=0.0f;
  dBodySetRotation( (dBodyID)b,m );
}

// ODE_API void dBodySetQuaternion (dBodyID, const dQuaternion q);
// ODE_API void dBodySetLinearVel  (dBodyID, dReal x, dReal y, dReal z);
// ODE_API void dBodySetAngularVel (dBodyID, dReal x, dReal y, dReal z);
// ODE_API const dReal * dBodyGetPosition (dBodyID);
// ODE_API void dBodyCopyPosition (dBodyID body, dVector3 pos);
// ODE_API const dReal * dBodyGetRotation (dBodyID);
// ODE_API void dBodyCopyRotation (dBodyID, dMatrix3 R);
// ODE_API const dReal * dBodyGetQuaternion (dBodyID);
// ODE_API void dBodyCopyQuaternion(dBodyID body, dQuaternion quat);
// ODE_API const dReal * dBodyGetLinearVel (dBodyID);
// ODE_API const dReal * dBodyGetAngularVel (dBodyID);
// ODE_API void dBodySetMass (dBodyID, const dMass *mass);
// ODE_API void dBodyGetMass (dBodyID, dMass *mass);
// ODE_API void dBodyAddForce            (dBodyID, dReal fx, dReal fy, dReal fz);
// ODE_API void dBodyAddTorque           (dBodyID, dReal fx, dReal fy, dReal fz);
// ODE_API void dBodyAddRelForce         (dBodyID, dReal fx, dReal fy, dReal fz);
// ODE_API void dBodyAddRelTorque        (dBodyID, dReal fx, dReal fy, dReal fz);
// ODE_API void dBodyAddForceAtPos       (dBodyID, dReal fx, dReal fy, dReal fz, dReal px, dReal py, dReal pz);
// ODE_API void dBodyAddForceAtRelPos    (dBodyID, dReal fx, dReal fy, dReal fz, dReal px, dReal py, dReal pz);
// ODE_API void dBodyAddRelForceAtPos    (dBodyID, dReal fx, dReal fy, dReal fz, dReal px, dReal py, dReal pz);
// ODE_API void dBodyAddRelForceAtRelPos (dBodyID, dReal fx, dReal fy, dReal fz, dReal px, dReal py, dReal pz);
// ODE_API const dReal * dBodyGetForce (dBodyID);
// ODE_API const dReal * dBodyGetTorque (dBodyID);
// ODE_API void dBodySetForce  (dBodyID b, dReal x, dReal y, dReal z);
// ODE_API void dBodySetTorque (dBodyID b, dReal x, dReal y, dReal z);
// ODE_API void dBodyGetRelPointPos (dBodyID, dReal px, dReal py, dReal pz, dVector3 result );
// ODE_API void dBodyGetRelPointVel( dBodyID, dReal px, dReal py, dReal pz, dVector3 result );
// ODE_API void dBodyGetPointVel(dBodyID, dReal px, dReal py, dReal pz, dVector3 result);
// ODE_API void dBodyGetPosRelPoint(dBodyID, dReal px, dReal py, dReal pz, dVector3 result );
// ODE_API void dBodyVectorToWorld(dBodyID, dReal px, dReal py, dReal pz,dVector3 result);
// ODE_API void dBodyVectorFromWorld(dBodyID, dReal px, dReal py, dReal pz,dVector3 result);
// ODE_API void dBodySetFiniteRotationMode (dBodyID, int mode);
// ODE_API void dBodySetFiniteRotationAxis (dBodyID, dReal x, dReal y, dReal z);
// ODE_API int dBodyGetFiniteRotationMode (dBodyID);
// ODE_API void dBodyGetFiniteRotationAxis (dBodyID, dVector3 result);
// ODE_API int dBodyGetNumJoints (dBodyID b);
// ODE_API dJointID dBodyGetJoint (dBodyID, int index);
// ODE_API void dBodySetDynamic (dBodyID);
// ODE_API void dBodySetKinematic (dBodyID);
// ODE_API int dBodyIsKinematic (dBodyID);
// ODE_API void dBodyEnable (dBodyID);
// ODE_API void dBodyDisable (dBodyID);
// ODE_API int dBodyIsEnabled (dBodyID);
// ODE_API void dBodySetGravityMode (dBodyID b, int mode);
// ODE_API int dBodyGetGravityMode (dBodyID b);
// ODE_API void dBodySetMovedCallback(dBodyID b, void (*callback)(dBodyID));
// ODE_API dGeomID dBodyGetFirstGeom (dBodyID b);
// ODE_API dGeomID dBodyGetNextGeom (dGeomID g);
// ODE_API void dBodySetDampingDefaults(dBodyID b);
// ODE_API dReal dBodyGetLinearDamping (dBodyID b);
// ODE_API void dBodySetLinearDamping(dBodyID b, dReal scale);
// ODE_API dReal dBodyGetAngularDamping (dBodyID b);
// ODE_API void dBodySetAngularDamping(dBodyID b, dReal scale);
// ODE_API void dBodySetDamping(dBodyID b, dReal linear_scale, dReal angular_scale);
// ODE_API dReal dBodyGetLinearDampingThreshold (dBodyID b);
// ODE_API void dBodySetLinearDampingThreshold(dBodyID b, dReal threshold);
// ODE_API dReal dBodyGetAngularDampingThreshold (dBodyID b);
// ODE_API void dBodySetAngularDampingThreshold(dBodyID b, dReal threshold);
// ODE_API dReal dBodyGetMaxAngularSpeed (dBodyID b);
// ODE_API void dBodySetMaxAngularSpeed(dBodyID b, dReal max_speed);
// ODE_API int dBodyGetGyroscopicMode(dBodyID b);
// ODE_API void dBodySetGyroscopicMode(dBodyID b, int enabled);
