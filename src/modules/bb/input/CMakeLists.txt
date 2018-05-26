bb_start_module(input)
set(DEPENDS_ON bb.runtime bb.system)
set(SOURCES input.cpp input.h driver.h device.cpp device.h module.link.cpp)
bb_end_module()
