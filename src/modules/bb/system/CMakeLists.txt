add_library (bb.system STATIC system.cpp system.h)
target_link_libraries (bb.system bb.blitz bb.runtime)
