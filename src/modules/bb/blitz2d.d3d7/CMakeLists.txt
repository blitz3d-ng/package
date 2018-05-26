IF(BB_WINDOWS)
  bb_start_module(blitz2d.d3d7)

  set(DEPENDS_ON bb.blitz2d)
  set(SOURCES blitz2d.d3d7.cpp blitz2d.d3d7.h font.cpp font.h)

  bb_end_module()
ENDIF()
