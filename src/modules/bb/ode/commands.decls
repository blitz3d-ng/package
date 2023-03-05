;Initialization
dInitODE():"_odeInitODE"
dCloseODE():"_odeCloseODE"

;World
dWorldCreate%():"_odeWorldCreate"
dWorldDestroy( world% ):"_odeWorldDestroy"
dWorldSetGravity( world%,x#,y#,z# ):"_odeWorldSetGravity"
dWorldQuickStep%( world%,stepsize# ):"_odeWorldQuickStep"
dWorldSetAutoDisableFlag( world%,do_auto_disable% ):"_odeWorldSetAutoDisableFlag"

;Space
dHashSpaceCreate%( space% ):"_odeHashSpaceCreate"
dSpaceDestroy( space% ):"_odeSpaceDestroy"

;Body
dBodySetAutoDisableFlag( body%,do_auto_disable% ):"_odeBodySetAutoDisableFlag"
dBodyCreate%( world% ):"_odeBodyCreate"
dBodySetPosition( body%,x#,y#,z# ):"_odeBodySetPosition"
dBodySetRotation( body%,pitch#,yaw#,roll# ):"_odeBodySetRotation"

;Geom
dGeomSetBody( geom%,body% ):"_odeGeomSetBody"
dGeomGetPositionX#( geom% ):"_odeGeomGetPositionX"
dGeomGetPositionY#( geom% ):"_odeGeomGetPositionY"
dGeomGetPositionZ#( geom% ):"_odeGeomGetPositionZ"
dGeomGetPitch#( geom% ):"_odeGeomGetPitch"
dGeomGetYaw#( geom% ):"_odeGeomGetYaw"
dGeomGetRoll#( geom% ):"_odeGeomGetRoll"

dCreateBox%( space%,lx#,ly#,lz# ):"_odeCreateBox"
dCreatePlane%( space%,a#,b#,c#,d# ):"_odeCreatePlane"

;Joints
dJointGroupCreate%( max_size% ):"_odeJointGroupCreate"
dJointGroupDestroy( group% ):"_odeJointGroupDestroy"
dJointGroupEmpty( group% ):"_odeJointGroupEmpty"

;Collision
dSpaceCollide( space%,world%,group% ):"_odeSpaceCollide"

;Contact
dContactSetMaxContacts( max% ):"_odeContactSetMaxContacts"
dContactGetMaxContacts%():"_odeContactGetMaxContacts"
dContactSetMode( mode% ):"_odeContactSetMode"
dContactGetMode%():"_odeContactGetMode"
dContactSetMu( mu# ):"_odeContactSetMu"
dContactGetMu#():"_odeContactGetMu"
dContactSetMu2( mu# ):"_odeContactSetMu2"
dContactGetMu2#():"_odeContactGetMu2"
dContactSetFDir1( fdir1x#,fdir1y#,fdir1z# ):"_odeContactSetFDir1"
dContactGetFDir1():"_odeContactGetFDir1"
dContactSetBounce( bounce# ):"_odeContactSetBounce"
dContactGetBounce#():"_odeContactGetBounce"
dContactSetBounceVel( vel# ):"_odeContactSetBounceVel"
dContactGetBounceVel#():"_odeContactGetBounceVel"
dContactSetSoftERP( erp# ):"_odeContactSetSoftERP"
dContactGetSoftERP#():"_odeContactGetSoftERP"
dContactSetSoftCFM( cfm# ):"_odeContactSetSoftCFM"
dContactGetSoftCFM#():"_odeContactGetSoftCFM"
dContactSetMotion1( motion# ):"_odeContactSetMotion1"
dContactGetMotion1#():"_odeContactGetMotion1"
dContactSetMotion2( motion# ):"_odeContactSetMotion2"
dContactGetMotion2#():"_odeContactGetMotion2"
dContactSetSlip1( slip# ):"_odeContactSetSlip1"
dContactGetSlip1#():"_odeContactGetSlip1"
dContactSetSlip2( slip# ):"_odeContactSetSlip2"
dContactGetSlip2#():"_odeContactGetSlip2"

;Blitz3D Helpers
dGeomSyncEntity( geom%,entity% ):"_odeGeomSyncEntity"

;JV-ODE Compatibility
dRegisterODE( key1%,key2%,key3%,key4% ):"_odeRegisterODE"
dGetVersion#():"_odeGetVersion"
dDemoTime%():"_odeDemoTime"
