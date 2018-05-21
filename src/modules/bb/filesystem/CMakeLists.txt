add_library (bb.filesystem STATIC filesystem.cpp filesystem.h)
target_link_libraries (bb.filesystem bb.blitz bb.stream)
