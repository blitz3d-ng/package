add_library (bb.bank STATIC bank.cpp bank.h)
target_link_libraries (bb.bank bb.blitz bb.stream)
