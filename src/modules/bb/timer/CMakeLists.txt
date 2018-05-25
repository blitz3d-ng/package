add_library (bb.timer STATIC timer.cpp timer.h module.link.cpp)
target_link_libraries (bb.timer bb.blitz bb.runtime)
