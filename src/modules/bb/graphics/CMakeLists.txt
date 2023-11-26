bb_start_module(graphics)
set(DEPENDS_ON bb.blitz bb.runtime bb.system bb.input bb.pixmap)
set(SOURCES graphics.h graphics.cpp canvas.h canvas.cpp driver.cpp font.h font.cpp movie.h movie.cpp)
set(LIBS freetype)
bb_end_module()

target_link_libraries(bb.graphics utf8.h)
