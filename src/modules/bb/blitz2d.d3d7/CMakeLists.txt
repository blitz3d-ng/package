IF(BB_WINDOWS)
  add_library (bb.blitz2d.d3d7 STATIC blitz2d.d3d7.cpp blitz2d.d3d7.h font.cpp font.h)
  target_link_libraries (bb.blitz2d.d3d7 bb.blitz2d)
ENDIF()
