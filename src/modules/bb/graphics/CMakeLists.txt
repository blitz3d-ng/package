bb_start_module(graphics)
set(DEPENDS_ON bb.blitz bb.runtime bb.system bb.input bb.pixmap bb.blitz2d)
set(SOURCES graphics.h graphics.cpp canvas.h canvas.cpp module.link.cpp)
bb_end_module()
