add_library (bb.system.macos STATIC system.macos.cpp system.macos.h lookup_font_file.m)
target_link_libraries (bb.system.macos bb.blitz bb.runtime bb.system "-framework Cocoa")
