bb_start_module(string)
set(DEPENDS_ON bb.blitz)
set(SOURCES string.cpp string.h)
bb_end_module()

target_link_libraries(bb.string utf8.h)
