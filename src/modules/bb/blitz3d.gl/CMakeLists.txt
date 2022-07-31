bin2h(SOURCE_FILE default.glsl HEADER_FILE default.glsl.h VARIABLE_NAME DEFAULT_GLSL)

bb_start_module(blitz3d.gl)
set(DEPENDS_ON bb.blitz3d bb.blitz2d.gl)
set(SOURCES blitz3d.gl.cpp blitz3d.gl.h default.glsl.h)
bb_end_module()
