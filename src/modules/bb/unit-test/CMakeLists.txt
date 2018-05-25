add_library (bb.unit-test STATIC unit-test.cpp unit-test.h module.link.cpp)
target_link_libraries (bb.unit-test bb.blitz)
