add_library (bb.filesystem.posix STATIC filesystem.posix.cpp filesystem.posix.h commands.h)
target_link_libraries (bb.filesystem.posix bb.blitz bb.stream bb.filesystem)
