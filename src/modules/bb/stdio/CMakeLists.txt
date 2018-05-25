add_library (bb.stdio STATIC stdio.cpp commands.h module.link.cpp)
target_link_libraries (bb.stdio bb.blitz)
