bb_start_module(graphics)
set(DEPENDS_ON bb.blitz bb.runtime bb.system bb.input bb.pixmap)
set(SOURCES graphics.h graphics.cpp canvas.h canvas.cpp)
set(LIBS freetype2) # TODO: remove once the cyclic dependency on blitz2d is done
bb_end_module()
