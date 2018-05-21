add_library (bb.pixmap STATIC pixmap.cpp pixmap.h)
target_include_directories (bb.pixmap PRIVATE SYSTEM ../../../deps/freeimage317/src/Source)
target_link_libraries (bb.pixmap bb.blitz freeimage)
