add_library (bb.blitz2d.gl STATIC blitz2d.gl.cpp blitz2d.gl.h)
target_include_directories (bb.blitz2d.gl PRIVATE SYSTEM ../../../deps/glew/include ../../../deps/freetype2/src/include)
target_link_libraries (bb.blitz2d.gl bb.blitz bb.pixmap bb.blitz2d)
