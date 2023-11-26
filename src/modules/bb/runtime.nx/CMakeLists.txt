if(NOT BB_NX)
  return()
endif()

bb_start_module(runtime.nx)
set(DEPENDS_ON bb.event bb.runtime)
set(SOURCES runtime.nx.cpp runtime.nx.h)
set(SYSTEM_LIBS nx)
bb_end_module()

target_include_directories(bb.runtime.nx PUBLIC /opt/devkitpro/libnx/include)
