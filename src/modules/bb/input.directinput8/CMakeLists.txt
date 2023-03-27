IF(BB_WINDOWS)
  bb_start_module(input.directinput8)
  set(DEPENDS_ON bb.input)
  set(SOURCES driver.cpp driver.h input.directinput8.h)
  set(LIBS dinput8 dxguid)
  bb_end_module()

  target_compile_definitions(bb.input.directinput8 PUBLIC DIRECTINPUT_VERSION=0x800)
ENDIF()
