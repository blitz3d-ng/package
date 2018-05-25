IF(BB_MOBILE)
  add_library (bb.blitz3d.gles2 STATIC blitz3d.gles2.cpp blitz3d.gles2.h)
  target_link_libraries (bb.blitz3d.gles2 bb.blitz3d bb.blitz2d.gles2)
ENDIF()
