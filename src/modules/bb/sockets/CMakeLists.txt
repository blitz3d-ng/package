IF(BB_WINDOWS)
  add_library (bb.sockets STATIC sockets.cpp sockets.h module.link.cpp)
  target_link_libraries (bb.sockets bb.stream wsock32)
ENDIF()
