IF(BB_MOBILE)
  bin2h(SOURCE_FILE simple.frag HEADER_FILE simple.frag.h VARIABLE_NAME SIMPLE2_FRAG)
  bin2h(SOURCE_FILE simple.vert HEADER_FILE simple.vert.h VARIABLE_NAME SIMPLE2_VERT)

  add_library (bb.blitz3d.gles2 STATIC blitz3d.gles2.cpp blitz3d.gles2.h simple.frag.h simple.vert.h)
  target_link_libraries (bb.blitz3d.gles2 bb.blitz3d bb.blitz2d.gles2)
ENDIF()
