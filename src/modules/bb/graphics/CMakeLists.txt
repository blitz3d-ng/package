add_library (bb.graphics STATIC graphics.h graphics.cpp canvas.h canvas.cpp)
target_include_directories (bb.graphics PRIVATE SYSTEM ../../../deps/freetype2/src/include)
target_link_libraries (bb.graphics bb.blitz bb.runtime bb.system bb.input bb.pixmap bb.blitz2d)
