IF(TARGET enet)
  bb_start_module(multiplay.enet)
  set(DEPENDS_ON bb.blitz bb.hook)
  set(SOURCES commands.h multiplay.enet.cpp multiplay.enet.h)
  set(LIBS enet crossguid)

  IF(BB_WINDOWS)
    set(LIBS ${LIBS} ws2_32)
  ELSEIF(BB_LINUX)
    set(LIBS ${LIBS} uuid)
  ENDIF()

  bb_end_module()
ENDIF()
