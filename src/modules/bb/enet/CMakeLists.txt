set(MODULES bb.blitz bb.hook)
set(LIBS enet crossguid)

IF(BB_WINDOWS)
  set(LIBS ${LIBS} Ws2_32)
ENDIF()

add_library (bb.enet STATIC commands.h enet.cpp enet.h module.link.cpp)
target_include_directories (bb.enet PRIVATE SYSTEM ../../../deps/enet/src/include)
target_link_libraries (bb.enet ${MODULES} ${LIBS})
