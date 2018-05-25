IF(BB_WINDOWS)
  set(MODULES bb.system.windows bb.blitz3d.d3d7 bb.frame)

  IF(BB_WIN32 OR BB_WIN64)
    set(LIBS ${LIBS} amstrmid)
  ELSE()
    set(LIBS ${LIBS} strmiids)
  ENDIF()

  add_library (bb.runtime.windows STATIC gxgraphics.cpp gxgraphics.h gxmovie.cpp gxmovie.h gxruntime.cpp gxruntime.h module.cpp std.h)
  target_link_libraries (bb.runtime.windows ${MODULES})
ENDIF()
