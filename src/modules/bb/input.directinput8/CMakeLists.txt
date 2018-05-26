IF(BB_WINDOWS)
  bb_start_module(ode)
  set(DEPENDS_ON bb.input)
  set(SOURCES driver.cpp driver.h input.directinput8.h)
  set(LIBS dinput8 dxguid)

  bb_end_module()
ENDIF()
