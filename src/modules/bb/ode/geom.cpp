
#include "ode.h"
#include <ode/ode.h>
#include <bb/blitz3d/geom.h>
#include "common.h"

// ODE_API void dGeomDestroy (dGeomID geom);
// ODE_API void dGeomSetData (dGeomID geom, void* data);
// ODE_API void *dGeomGetData (dGeomID geom);

BBLIB void BBCALL _odeGeomSetBody( bb_ptr_t g,bb_ptr_t b ){
  dGeomSetBody( (dGeomID)g,(dBodyID)b );
}

// ODE_API dBodyID dGeomGetBody (dGeomID geom);
// ODE_API void dGeomSetPosition (dGeomID geom, dReal x, dReal y, dReal z);
// ODE_API void dGeomSetRotation (dGeomID geom, const dMatrix3 R);
// ODE_API void dGeomSetQuaternion (dGeomID geom, const dQuaternion Q);

BBLIB bb_float_t BBCALL _odeGeomGetPositionX( bb_ptr_t g ){
  return dGeomGetPosition( (dGeomID)g )[0];
}

BBLIB bb_float_t BBCALL _odeGeomGetPositionY( bb_ptr_t g ){
  return dGeomGetPosition( (dGeomID)g )[1];
}

BBLIB bb_float_t BBCALL _odeGeomGetPositionZ( bb_ptr_t g ){
  return dGeomGetPosition( (dGeomID)g )[2];
}

// ODE_API void dGeomCopyPosition (dGeomID geom, dVector3 pos);

void get_euler(const dReal * matrix,dReal &kx,dReal &ky,dReal &kz)
{
const dReal epsilon=0.0000001;

if(matrix[8] < 1-epsilon && matrix[8] > -1+epsilon){
        ky=-asin(matrix[8]);
        dReal c=cos(ky);
        kx= atan2(matrix[9]/c,matrix[10]/c);
        kz= atan2(matrix[4]/c,matrix[0]/c);
        }
else
        {
        kz=0;
        ky=-atan2(matrix[8],0);
        kx= atan2(-matrix[6],matrix[5]);
        }
}


BBLIB bb_float_t BBCALL _odeGeomGetPitch( bb_ptr_t g ){
  float p,y,r;
  const dReal *m=dGeomGetRotation( (dGeomID)g );
  get_euler( m,p,y,r );
  return r*rtod;
  // dQuaternion dq;
  // dGeomGetQuaternion( (dGeomID)g,dq );
  //
	// Quat q( dq[0],&dq[1] );
	// return quatPitch( q ) * rtod;
}

BBLIB bb_float_t BBCALL _odeGeomGetYaw( bb_ptr_t g ){
  float p,y,r;
  const dReal *m=dGeomGetRotation( (dGeomID)g );
  get_euler( m,p,y,r );
  return y*rtod;
  // dQuaternion dq;
  // dGeomGetQuaternion( (dGeomID)g,dq );
  //
	// Quat q( dq[0],&dq[1] );
	// return quatYaw( q ) * rtod;
}

BBLIB bb_float_t BBCALL _odeGeomGetRoll( bb_ptr_t g ){
  float p,y,r;
  const dReal *m=dGeomGetRotation( (dGeomID)g );
  get_euler( m,p,y,r );
  return p*rtod;
  // dQuaternion dq;
  // dGeomGetQuaternion( (dGeomID)g,dq );
  //
	// Quat q( dq[0],&dq[1] );
	// return quatRoll( q ) * rtod;
}

// ODE_API void dGeomCopyRotation(dGeomID geom, dMatrix3 R);
// ODE_API void dGeomGetQuaternion (dGeomID geom, dQuaternion result);
// ODE_API void dGeomGetAABB (dGeomID geom, dReal aabb[6]);
// ODE_API int dGeomIsSpace (dGeomID geom);
// ODE_API dSpaceID dGeomGetSpace (dGeomID);
// ODE_API int dGeomGetClass (dGeomID geom);
// ODE_API void dGeomSetCategoryBits (dGeomID geom, unsigned long bits);
// ODE_API void dGeomSetCollideBits (dGeomID geom, unsigned long bits);
// ODE_API unsigned long dGeomGetCategoryBits (dGeomID);
// ODE_API unsigned long dGeomGetCollideBits (dGeomID);
// ODE_API void dGeomEnable (dGeomID geom);
// ODE_API void dGeomDisable (dGeomID geom);
// ODE_API int dGeomIsEnabled (dGeomID geom);
// ODE_API int dGeomLowLevelControl (dGeomID geom, int controlClass, int controlCode, void *dataValue, int *dataSize);
// ODE_API void dGeomGetRelPointPos (dGeomID geom, dReal px, dReal py, dReal pz, dVector3 result);
// ODE_API void dGeomGetPosRelPoint (dGeomID geom, dReal px, dReal py, dReal pz,dVector3 result);
// ODE_API void dGeomVectorToWorld (dGeomID geom, dReal px, dReal py, dReal pz,dVector3 result);
// ODE_API void dGeomVectorFromWorld(dGeomID geom, dReal px, dReal py, dReal pz,dVector3 result);
// ODE_API void dGeomSetOffsetPosition (dGeomID geom, dReal x, dReal y, dReal z);
// ODE_API void dGeomSetOffsetRotation (dGeomID geom, const dMatrix3 R);
// ODE_API void dGeomSetOffsetQuaternion (dGeomID geom, const dQuaternion Q);
// ODE_API void dGeomSetOffsetWorldPosition (dGeomID geom, dReal x, dReal y, dReal z);
// ODE_API void dGeomSetOffsetWorldRotation (dGeomID geom, const dMatrix3 R);
// ODE_API void dGeomSetOffsetWorldQuaternion (dGeomID geom, const dQuaternion);
// ODE_API void dGeomClearOffset(dGeomID geom);
// ODE_API int dGeomIsOffset(dGeomID geom);
// ODE_API const dReal * dGeomGetOffsetPosition (dGeomID geom);
// ODE_API void dGeomCopyOffsetPosition (dGeomID geom, dVector3 pos);
// ODE_API const dReal * dGeomGetOffsetRotation (dGeomID geom);
// ODE_API void dGeomCopyOffsetRotation (dGeomID geom, dMatrix3 R);
// ODE_API void dGeomGetOffsetQuaternion (dGeomID geom, dQuaternion result);

BBLIB bb_ptr_t BBCALL _odeCreateBox( bb_ptr_t s,bb_float_t lx,bb_float_t ly,bb_float_t lz ){
  return (bb_ptr_t)dCreateBox( (dSpaceID)s,lx,ly,lz );
}

// ODE_API void dGeomBoxSetLengths (dGeomID box, dReal lx, dReal ly, dReal lz);
// ODE_API void dGeomBoxGetLengths (dGeomID box, dVector3 result);
// ODE_API dReal dGeomBoxPointDepth (dGeomID box, dReal x, dReal y, dReal z);

BBLIB bb_ptr_t BBCALL _odeCreatePlane( bb_ptr_t s,bb_float_t a,bb_float_t b,bb_float_t c,bb_float_t d ){
  return (bb_ptr_t)dCreatePlane( (dSpaceID)s,a,b,c,d );
}

// ODE_API void dGeomPlaneSetParams (dGeomID plane, dReal a, dReal b, dReal c, dReal d);
// ODE_API void dGeomPlaneGetParams (dGeomID plane, dVector4 result);
// ODE_API dReal dGeomPlanePointDepth (dGeomID plane, dReal x, dReal y, dReal z);
