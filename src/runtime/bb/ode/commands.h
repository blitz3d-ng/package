#ifndef BB_ODE_COMMANDS_H
#define BB_ODE_COMMANDS_H

#include <bb/blitz3d/commands.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialization
void     BBCALL _odeInitODE();
void     BBCALL _odeCloseODE();

// World
bb_ptr_t BBCALL _odeWorldCreate();
void     BBCALL _odeWorldDestroy( bb_ptr_t w );
void     BBCALL _odeWorldSetGravity( bb_ptr_t w,float x,float y,float z);
int      BBCALL _odeWorldQuickStep( bb_ptr_t w,float stepsize );
void     BBCALL _odeWorldSetAutoDisableFlag( bb_ptr_t w,int do_auto_disable);

// Space
bb_ptr_t BBCALL _odeHashSpaceCreate( bb_ptr_t s );
void     BBCALL _odeSpaceDestroy( bb_ptr_t s );

// Body
void     BBCALL _odeBodySetAutoDisableFlag( bb_ptr_t b,int do_auto_disable );
bb_ptr_t BBCALL _odeBodyCreate( bb_ptr_t w );
void     BBCALL _odeBodySetPosition( bb_ptr_t b,float x,float y,float z );
void     BBCALL _odeBodySetRotation( bb_ptr_t b,float p,float y,float r );

// Geom
void     BBCALL _odeGeomSetBody( bb_ptr_t g,bb_ptr_t b );
float    BBCALL _odeGeomGetPositionX( bb_ptr_t g );
float    BBCALL _odeGeomGetPositionY( bb_ptr_t g );
float    BBCALL _odeGeomGetPositionZ( bb_ptr_t g );
float    BBCALL _odeGeomGetPitch( bb_ptr_t g );
float    BBCALL _odeGeomGetYaw( bb_ptr_t g );
float    BBCALL _odeGeomGetRoll( bb_ptr_t g );

bb_ptr_t BBCALL _odeCreateBox( bb_ptr_t s,float lx,float ly,float lz );
bb_ptr_t BBCALL _odeCreatePlane( bb_ptr_t s,float a,float b,float c,float d );

// Joints
bb_ptr_t BBCALL _odeJointGroupCreate( int max_size );
void     BBCALL _odeJointGroupDestroy( bb_ptr_t g );
void     BBCALL _odeJointGroupEmpty( bb_ptr_t g );

// Collision
void     BBCALL _odeSpaceCollide( bb_ptr_t s,bb_ptr_t w,bb_ptr_t g );

// Contact
void  BBCALL _odeContactSetMaxContacts( int max_contacts );
int   BBCALL _odeContactGetMaxContacts();
void  BBCALL _odeContactSetMode( int mode );
int   BBCALL _odeContactGetMode();
void  BBCALL _odeContactSetMu( float mu );
float BBCALL _odeContactGetMu();
void  BBCALL _odeContactSetMu2( float mu );
float BBCALL _odeContactGetMu2();
void  BBCALL _odeContactSetFDir1( float fdir1x,float fdir1y,float fdir1z );
void  BBCALL _odeContactGetFDir1();
void  BBCALL _odeContactSetBounce( float bounce );
float BBCALL _odeContactGetBounce();
void  BBCALL _odeContactSetBounceVel( float vel );
float BBCALL _odeContactGetBounceVel();
void  BBCALL _odeContactSetSoftERP( float erp );
float BBCALL _odeContactGetSoftERP();
void  BBCALL _odeContactSetSoftCFM( float cfm );
float BBCALL _odeContactGetSoftCFM();
void  BBCALL _odeContactSetMotion1( float motion );
float BBCALL _odeContactGetMotion1();
void  BBCALL _odeContactSetMotion2( float motion );
float BBCALL _odeContactGetMotion2();
void  BBCALL _odeContactSetSlip1( float slip );
float BBCALL _odeContactGetSlip1();
void  BBCALL _odeContactSetSlip2( float slip );
float BBCALL _odeContactGetSlip2();

// Blitz3D Helpers
void BBCALL _odeGeomSyncEntity( bb_ptr_t g,Entity *e );

// JV-ODE compatibility
void     BBCALL _odeRegisterODE( int key1,int key2,int key3,int key4 );
float    BBCALL _odeGetVersion();

#ifdef __cplusplus
}
#endif

#endif
