add_library (bb.input STATIC input.cpp input.h driver.h device.cpp device.h)
target_link_libraries (bb.input bb.blitz bb.runtime bb.system)
