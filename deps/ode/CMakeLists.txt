add_library(ode STATIC tree/ode/src/array.cpp tree/ode/src/collision_trimesh_box.cpp tree/ode/src/error.cpp tree/ode/src/odeou.cpp tree/ode/src/box.cpp tree/ode/src/collision_trimesh_ccylinder.cpp tree/ode/src/export-dif.cpp tree/ode/src/odetls.cpp tree/ode/src/capsule.cpp tree/ode/src/collision_trimesh_disabled.cpp tree/ode/src/heightfield.cpp tree/ode/src/plane.cpp tree/ode/src/collision_convex_trimesh.cpp tree/ode/src/collision_trimesh_distance.cpp tree/ode/src/lcp.cpp tree/ode/src/quickstep.cpp tree/ode/src/collision_cylinder_box.cpp tree/ode/src/collision_trimesh_gimpact.cpp tree/ode/src/mass.cpp tree/ode/src/ray.cpp tree/ode/src/collision_cylinder_plane.cpp tree/ode/src/collision_trimesh_opcode.cpp tree/ode/src/mat.cpp tree/ode/src/rotation.cpp tree/ode/src/collision_cylinder_sphere.cpp tree/ode/src/collision_trimesh_plane.cpp tree/ode/src/matrix.cpp tree/ode/src/sphere.cpp tree/ode/src/collision_cylinder_trimesh.cpp tree/ode/src/collision_trimesh_ray.cpp tree/ode/src/memory.cpp tree/ode/src/step.cpp tree/ode/src/collision_kernel.cpp tree/ode/src/collision_trimesh_sphere.cpp tree/ode/src/misc.cpp tree/ode/src/threading_base.cpp tree/ode/src/collision_libccd.cpp tree/ode/src/collision_trimesh_trimesh.cpp tree/ode/src/objects.cpp tree/ode/src/threading_impl.cpp tree/ode/src/collision_quadtreespace.cpp tree/ode/src/collision_trimesh_trimesh_new.cpp tree/ode/src/obstack.cpp tree/ode/src/threading_pool_posix.cpp tree/ode/src/collision_sapspace.cpp tree/ode/src/collision_util.cpp tree/ode/src/ode.cpp tree/ode/src/threading_pool_win.cpp tree/ode/src/collision_space.cpp tree/ode/src/convex.cpp tree/ode/src/odeinit.cpp tree/ode/src/timer.cpp tree/ode/src/collision_transform.cpp tree/ode/src/cylinder.cpp tree/ode/src/odemath.cpp tree/ode/src/util.cpp tree/ode/src/fastdot.c tree/ode/src/fastldlt.c tree/ode/src/fastlsolve.c tree/ode/src/fastltsolve.c tree/ode/src/nextafterf.c tree/ode/src/array.h tree/ode/src/collision_util.h tree/ode/src/odemath.h tree/ode/src/threading_impl.h tree/ode/src/collision_kernel.h tree/ode/src/error.h tree/ode/src/odeou.h tree/ode/src/threading_impl_posix.h tree/ode/src/collision_libccd.h tree/ode/src/heightfield.h tree/ode/src/odetls.h tree/ode/src/threading_impl_templates.h tree/ode/src/collision_space_internal.h tree/ode/src/lcp.h tree/ode/src/quickstep.h tree/ode/src/threading_impl_win.h tree/ode/src/collision_std.h tree/ode/src/mat.h tree/ode/src/step.h tree/ode/src/threadingutils.h tree/ode/src/collision_transform.h tree/ode/src/matrix.h tree/ode/src/threading_atomics_provs.h tree/ode/src/typedefs.h tree/ode/src/collision_trimesh_colliders.h tree/ode/src/objects.h tree/ode/src/threading_base.h tree/ode/src/util.h tree/ode/src/collision_trimesh_internal.h tree/ode/src/obstack.h tree/ode/src/threading_fake_sync.h tree/ode/src/joints/contact.cpp tree/ode/src/joints/dhinge.h tree/ode/src/joints/hinge2.cpp tree/ode/src/joints/joints.h tree/ode/src/joints/piston.cpp tree/ode/src/joints/pr.h tree/ode/src/joints/transmission.cpp tree/ode/src/joints/amotor.cpp tree/ode/src/joints/contact.h tree/ode/src/joints/fixed.cpp tree/ode/src/joints/hinge2.h tree/ode/src/joints/lmotor.cpp tree/ode/src/joints/piston.h tree/ode/src/joints/pu.cpp tree/ode/src/joints/transmission.h tree/ode/src/joints/amotor.h tree/ode/src/joints/dball.cpp tree/ode/src/joints/fixed.h tree/ode/src/joints/joint.cpp tree/ode/src/joints/lmotor.h tree/ode/src/joints/plane2d.cpp tree/ode/src/joints/pu.h tree/ode/src/joints/universal.cpp tree/ode/src/joints/ball.cpp tree/ode/src/joints/dball.h tree/ode/src/joints/hinge.cpp tree/ode/src/joints/joint.h tree/ode/src/joints/null.cpp tree/ode/src/joints/plane2d.h tree/ode/src/joints/slider.cpp tree/ode/src/joints/universal.h tree/ode/src/joints/ball.h tree/ode/src/joints/dhinge.cpp tree/ode/src/joints/hinge.h tree/ode/src/joints/joint_internal.h tree/ode/src/joints/null.h tree/ode/src/joints/pr.cpp tree/ode/src/joints/slider.h tree/OPCODE/OPC_AABBCollider.h tree/OPCODE/OPC_IceHook.h tree/OPCODE/OPC_OptimizedTree.h tree/OPCODE/OPC_RayTriOverlap.h tree/OPCODE/OPC_TriBoxOverlap.h tree/OPCODE/OPC_AABBTree.h tree/OPCODE/OPC_LSSAABBOverlap.h tree/OPCODE/OPC_Picking.h tree/OPCODE/OPC_Settings.h tree/OPCODE/OPC_TriTriOverlap.h tree/OPCODE/OPC_BaseModel.h tree/OPCODE/OPC_LSSCollider.h tree/OPCODE/OPC_PlanesAABBOverlap.h tree/OPCODE/OPC_SphereAABBOverlap.h tree/OPCODE/OPC_VolumeCollider.h tree/OPCODE/OPC_BoxBoxOverlap.h tree/OPCODE/OPC_LSSTriOverlap.h tree/OPCODE/OPC_PlanesCollider.h tree/OPCODE/OPC_SphereCollider.h tree/OPCODE/Opcode.h tree/OPCODE/OPC_Collider.h tree/OPCODE/OPC_MeshInterface.h tree/OPCODE/OPC_PlanesTriOverlap.h tree/OPCODE/OPC_SphereTriOverlap.h tree/OPCODE/Stdafx.h tree/OPCODE/OPC_Common.h tree/OPCODE/OPC_Model.h tree/OPCODE/OPC_RayAABBOverlap.h tree/OPCODE/OPC_TreeBuilders.h tree/OPCODE/OPC_HybridModel.h tree/OPCODE/OPC_OBBCollider.h tree/OPCODE/OPC_RayCollider.h tree/OPCODE/OPC_TreeCollider.h tree/OPCODE/OPC_AABBCollider.cpp tree/OPCODE/OPC_Common.cpp tree/OPCODE/OPC_Model.cpp tree/OPCODE/OPC_PlanesCollider.cpp tree/OPCODE/OPC_TreeCollider.cpp tree/OPCODE/OPC_AABBTree.cpp tree/OPCODE/OPC_HybridModel.cpp tree/OPCODE/OPC_OBBCollider.cpp tree/OPCODE/OPC_RayCollider.cpp tree/OPCODE/OPC_VolumeCollider.cpp tree/OPCODE/OPC_BaseModel.cpp tree/OPCODE/OPC_LSSCollider.cpp tree/OPCODE/OPC_OptimizedTree.cpp tree/OPCODE/OPC_SphereCollider.cpp tree/OPCODE/Opcode.cpp tree/OPCODE/OPC_Collider.cpp tree/OPCODE/OPC_MeshInterface.cpp tree/OPCODE/OPC_Picking.cpp tree/OPCODE/OPC_TreeBuilders.cpp tree/OPCODE/StdAfx.cpp tree/OPCODE/Ice/IceAABB.h tree/OPCODE/Ice/IceHPoint.h tree/OPCODE/Ice/IceMemoryMacros.h tree/OPCODE/Ice/IcePreprocessor.h tree/OPCODE/Ice/IceTriList.h tree/OPCODE/Ice/IceAxes.h tree/OPCODE/Ice/IceIndexedTriangle.h tree/OPCODE/Ice/IceOBB.h tree/OPCODE/Ice/IceRandom.h tree/OPCODE/Ice/IceTriangle.h tree/OPCODE/Ice/IceBoundingSphere.h tree/OPCODE/Ice/IceLSS.h tree/OPCODE/Ice/IcePairs.h tree/OPCODE/Ice/IceRay.h tree/OPCODE/Ice/IceTypes.h tree/OPCODE/Ice/IceContainer.h tree/OPCODE/Ice/IceMatrix3x3.h tree/OPCODE/Ice/IcePlane.h tree/OPCODE/Ice/IceRevisitedRadix.h tree/OPCODE/Ice/IceUtils.h tree/OPCODE/Ice/IceFPU.h tree/OPCODE/Ice/IceMatrix4x4.h tree/OPCODE/Ice/IcePoint.h tree/OPCODE/Ice/IceSegment.h tree/OPCODE/Ice/IceAABB.cpp tree/OPCODE/Ice/IceIndexedTriangle.cpp tree/OPCODE/Ice/IceOBB.cpp tree/OPCODE/Ice/IceRandom.cpp tree/OPCODE/Ice/IceSegment.cpp tree/OPCODE/Ice/IceContainer.cpp tree/OPCODE/Ice/IceMatrix3x3.cpp tree/OPCODE/Ice/IcePlane.cpp tree/OPCODE/Ice/IceRay.cpp tree/OPCODE/Ice/IceTriangle.cpp tree/OPCODE/Ice/IceHPoint.cpp tree/OPCODE/Ice/IceMatrix4x4.cpp tree/OPCODE/Ice/IcePoint.cpp tree/OPCODE/Ice/IceRevisitedRadix.cpp tree/OPCODE/Ice/IceUtils.cpp tree/ou/src/ou/atomic.cpp tree/ou/src/ou/customization.cpp tree/ou/src/ou/malloc.cpp tree/ou/src/ou/threadlocalstorage.cpp)

target_include_directories(ode SYSTEM PUBLIC include tree/include tree/ode/src tree/OPCODE tree/libccd/src tree/ou/include)

target_compile_definitions(ode PRIVATE -DdIDESINGLE -DCCD_IDESINGLE)
target_compile_definitions(ode PUBLIC -DODE_LIB)

IF(BB_MACOS)
  target_compile_definitions(ode PRIVATE -DMAC_OS_X_VERSION=1090)
ELSEIF(BB_MSVC)
  # suppress doube -> dReal warnings
  target_compile_options(ode PRIVATE /w4244)
ELSEIF(BB_NX)
  target_compile_definitions(ode PRIVATE __linux__ __unix__)
ENDIF()