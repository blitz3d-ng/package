IF(BB_WINDOWS)
  set(MODULES bb.input)
  set(LIBS dinput8 dxguid)

  add_library (bb.input.directinput8 STATIC driver.cpp driver.h input.directinput8.h)
  target_link_libraries (bb.input.directinput8 ${MODULES} ${LIBS})
ENDIF()
