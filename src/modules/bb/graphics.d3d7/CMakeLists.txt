IF(BB_WINDOWS)
  set(MODULES bb.graphics)
  set(LIBS ddraw)

  add_library (bb.graphics.d3d7 STATIC graphics.d3d7.h driver.cpp driver.h canvas.cpp canvas2.cpp canvas.h ddutil.cpp ddutil.h asmcoder.h)
  target_link_libraries (bb.graphics.d3d7 ${MODULES} ${LIBS})
ENDIF()
