add_library (bb.graphics STATIC graphics.h graphics.cpp canvas.h canvas.cpp module.link.cpp)
target_link_libraries (bb.graphics bb.blitz bb.runtime bb.system bb.input bb.pixmap bb.blitz2d)
