add_library (bb.blitz2d STATIC module.cpp font.h font.cpp graphics.h blitz2d.h movie.h movie.cpp module.link.cpp)
target_link_libraries (bb.blitz2d bb.blitz bb.pixmap freetype2)
