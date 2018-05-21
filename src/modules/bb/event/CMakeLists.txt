add_library (bb.event STATIC event.cpp event.h)
target_link_libraries (bb.event bb.blitz bb.hook)
