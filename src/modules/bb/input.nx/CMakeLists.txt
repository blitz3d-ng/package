if(NOT BB_NX)
  return()
endif()

bb_start_module(input.nx)
set(DEPENDS_ON bb.input)
set(SOURCES input.nx.h input.nx.cpp)
set(LIBS nx)
bb_end_module()
