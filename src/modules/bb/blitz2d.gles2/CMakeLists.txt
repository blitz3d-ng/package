IF(BB_MOBILE)
  add_library (bb.blitz2d.gles2 STATIC blitz2d.gles2.cpp blitz2d.gles2.h)
  target_link_libraries (bb.blitz2d.gles2 bb.blitz2d)
ENDIF()
