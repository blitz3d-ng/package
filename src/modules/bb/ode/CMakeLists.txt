add_library (bb.ode STATIC commands.h ode.h ode.cpp world.cpp space.cpp joints.cpp collision.cpp geom.cpp trimesh.cpp body.cpp jv-ode-compat.cpp)
target_link_libraries (bb.ode bb.blitz bb.pixmap bb.blitz2d bb.blitz3d ode)
