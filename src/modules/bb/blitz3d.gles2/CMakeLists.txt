IF(BB_MOBILE OR BB_WEB OR BB_NX)
  bin2h(SOURCE_FILE simple.frag HEADER_FILE simple.frag.h VARIABLE_NAME SIMPLE2_FRAG)
  bin2h(SOURCE_FILE simple.vert HEADER_FILE simple.vert.h VARIABLE_NAME SIMPLE2_VERT)

  bb_start_module(blitz3d.gles2)
  set(DEPENDS_ON bb.blitz3d bb.blitz2d.gles2)
  set(SOURCES blitz3d.gles2.cpp blitz3d.gles2.h simple.frag.h simple.vert.h)
  bb_end_module()
ENDIF()
