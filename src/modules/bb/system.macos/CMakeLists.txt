bb_start_module(system.macos)
set(DEPENDS_ON bb.runtime bb.system)
set(SOURCES system.macos.cpp system.macos.h lookup_font_file.m)
set(LIBS "-framework Cocoa")

bb_end_module()
