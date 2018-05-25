add_library (bb.bank STATIC bank.cpp bank.h module.link.cpp)
target_link_libraries (bb.bank bb.blitz bb.stream)
