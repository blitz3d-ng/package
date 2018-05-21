add_library (bb.blitz3d.gl STATIC blitz3d.gl.cpp blitz3d.gl.h)
target_include_directories (bb.blitz3d.gl PRIVATE SYSTEM ../../../deps/glew/include ../../../deps/freetype2/src/include)
target_link_libraries (bb.blitz3d.gl bb.blitz bb.pixmap bb.blitz2d bb.blitz3d bb.blitz2d.gl)
