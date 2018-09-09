bb_start_module(filesystem.posix)
set(DEPENDS_ON bb.stream bb.filesystem)
set(SOURCES filesystem.posix.cpp filesystem.posix.h commands.h)
bb_end_module()
