IF(BB_WINDOWS)
  bb_start_module(blitz2d.d3d7)

  set(DEPENDS_ON bb.blitz2d)
  set(SOURCES blitz2d.d3d7.cpp blitz2d.d3d7.h font.cpp font.h gxmovie.cpp gxmovie.h)

  if(BB_WIN32 OR BB_WIN64)
    set(LIBS ${LIBS} amstrmid)
  else()
    set(LIBS ${LIBS} strmiids)
  endif()

  bb_end_module()
ENDIF()
