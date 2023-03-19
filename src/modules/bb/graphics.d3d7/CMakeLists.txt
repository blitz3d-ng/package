IF(BB_WINDOWS)
  bb_start_module(graphics.d3d7)
  set(DEPENDS_ON bb.graphics)
  set(SOURCES graphics.d3d7.h driver.cpp driver.h canvas.cpp canvas2.cpp canvas.h ddutil.cpp ddutil.h asmcoder.h)
  set(LIBS ddraw dxguid freetype2)
  bb_end_module()

  set_target_properties(bb.graphics.d3d7 PROPERTIES UNITY_BUILD false)
ENDIF()
