bin2h(SOURCE_FILE default.glsl HEADER_FILE default.glsl.h VARIABLE_NAME DEFAULT_GLSL)

bb_start_module(blitz2d.gl)
set(DEPENDS_ON bb.blitz2d bb.graphics.gl)
set(SOURCES blitz2d.gl.cpp blitz2d.gl.h default.glsl.h)
bb_end_module()
