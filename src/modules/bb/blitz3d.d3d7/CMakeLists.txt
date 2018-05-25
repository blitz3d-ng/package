IF(BB_WINDOWS)
  add_library (bb.blitz3d.d3d7 STATIC blitz3d.d3d7.cpp light.cpp light.h mesh.cpp mesh.h scene.cpp scene.h)
  target_link_libraries (bb.blitz3d.d3d7 bb.blitz3d bb.blitz2d.d3d7 bb.graphics.d3d7)
ENDIF()
