bb_start_module(enet)
set(DEPENDS_ON bb.blitz bb.hook)
set(SOURCES commands.h enet.cpp enet.h module.link.cpp)
set(LIBS enet crossguid)

IF(BB_WINDOWS)
  set(LIBS ${LIBS} Ws2_32)
ENDIF()

bb_end_module()

target_include_directories (bb.enet PRIVATE SYSTEM ../../../deps/enet/src/include)
