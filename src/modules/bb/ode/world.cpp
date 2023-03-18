
#include "ode.h"
#include <ode/ode.h>

BBLIB bb_ptr_t BBCALL _odeWorldCreate(){
  return (bb_ptr_t)dWorldCreate();
}

BBLIB void BBCALL _odeWorldDestroy( bb_ptr_t w ){
  dWorldDestroy( (dWorldID)w );
}

// ODE_API void dWorldSetData (dWorldID world, void* data);
// ODE_API void* dWorldGetData (dWorldID world);

BBLIB void BBCALL _odeWorldSetGravity( bb_ptr_t w,bb_float_t x,bb_float_t y,bb_float_t z){
  dWorldSetGravity( (dWorldID)w,x,y,z );
}
// ODE_API void dWorldGetGravity (dWorldID, dVector3 gravity);
// ODE_API void dWorldSetERP (dWorldID, dReal erp);
// ODE_API dReal dWorldGetERP (dWorldID);
// ODE_API void dWorldSetCFM (dWorldID, dReal cfm);
// ODE_API dReal dWorldGetCFM (dWorldID);
// ODE_API void dWorldSetStepIslandsProcessingMaxThreadCount(dWorldID w, unsigned count);
// ODE_API unsigned dWorldGetStepIslandsProcessingMaxThreadCount(dWorldID w);
// ODE_API int dWorldUseSharedWorkingMemory(dWorldID w, dWorldID from_world/*=NULL*/);
// ODE_API void dWorldCleanupWorkingMemory(dWorldID w);
// ODE_API int dWorldSetStepMemoryReservationPolicy(dWorldID w, const dWorldStepReserveInfo *policyinfo/*=NULL*/);
// ODE_API int dWorldSetStepMemoryManager(dWorldID w, const dWorldStepMemoryFunctionsInfo *memfuncs);
// ODE_API void dWorldSetStepThreadingImplementation(dWorldID w, const dThreadingFunctionsInfo *functions_info, dThreadingImplementationID threading_impl);
// ODE_API int dWorldStep (dWorldID w, dReal stepsize);

BBLIB bb_int_t BBCALL _odeWorldQuickStep( bb_ptr_t w,bb_float_t stepsize ){
  return dWorldQuickStep( (dWorldID)w,stepsize );
}

// ODE_API void dWorldSetQuickStepNumIterations (dWorldID, int num);
// ODE_API int dWorldGetQuickStepNumIterations (dWorldID);
// ODE_API void dWorldSetQuickStepW (dWorldID, dReal over_relaxation);
// ODE_API dReal dWorldGetQuickStepW (dWorldID);
// ODE_API void dWorldSetContactMaxCorrectingVel (dWorldID, dReal vel);
// ODE_API dReal dWorldGetContactMaxCorrectingVel (dWorldID);
// ODE_API void dWorldSetContactSurfaceLayer (dWorldID, dReal depth);
// ODE_API dReal dWorldGetContactSurfaceLayer (dWorldID);
// ODE_API dReal dWorldGetAutoDisableLinearThreshold (dWorldID);
// ODE_API void  dWorldSetAutoDisableLinearThreshold (dWorldID, dReal linear_average_threshold);
// ODE_API dReal dWorldGetAutoDisableAngularThreshold (dWorldID);
// ODE_API void dWorldSetAutoDisableAngularThreshold (dWorldID, dReal angular_average_threshold);
// ODE_API int dWorldGetAutoDisableAverageSamplesCount (dWorldID);
// ODE_API void dWorldSetAutoDisableAverageSamplesCount (dWorldID, unsigned int average_samples_count );
// ODE_API int dWorldGetAutoDisableSteps (dWorldID);
// ODE_API void dWorldSetAutoDisableSteps (dWorldID, int steps);
// ODE_API dReal dWorldGetAutoDisableTime (dWorldID);
// ODE_API void dWorldSetAutoDisableTime (dWorldID, dReal time);
// ODE_API int dWorldGetAutoDisableFlag (dWorldID);

BBLIB void BBCALL _odeWorldSetAutoDisableFlag( bb_ptr_t w,bb_int_t do_auto_disable){
  dWorldSetAutoDisableFlag( (dWorldID)w,do_auto_disable );
}

// ODE_API dReal dWorldGetLinearDampingThreshold (dWorldID w);
// ODE_API void dWorldSetLinearDampingThreshold(dWorldID w, dReal threshold);
// ODE_API dReal dWorldGetAngularDampingThreshold (dWorldID w);
// ODE_API void dWorldSetAngularDampingThreshold(dWorldID w, dReal threshold);
// ODE_API dReal dWorldGetLinearDamping (dWorldID w);
// ODE_API void dWorldSetLinearDamping (dWorldID w, dReal scale);
// ODE_API dReal dWorldGetAngularDamping (dWorldID w);
// ODE_API void dWorldSetAngularDamping(dWorldID w, dReal scale);
// ODE_API void dWorldSetDamping (dWorldID w, dReal linear_scale, dReal angular_scale);
// ODE_API dReal dWorldGetMaxAngularSpeed (dWorldID w);
// ODE_API void dWorldSetMaxAngularSpeed (dWorldID w, dReal max_speed);
