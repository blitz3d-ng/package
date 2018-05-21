add_library (bb.runtime.console STATIC runtime.console.cpp)
target_link_libraries (bb.runtime.console bb.blitz bb.hook bb.event bb.runtime)
