IF(BB_WINDOWS)
  add_library (bb.userlibs STATIC userlibs.cpp userlibs.h)
  target_link_libraries (bb.userlibs bb.blitz)
ENDIF()
