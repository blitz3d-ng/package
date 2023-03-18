bb_start_runtime(test Test bbCreateConsoleRuntime)

set(EXCLUDED_MODULES
  blitz3d.d3d7
  blitz3d.gl
  blitz3d
  blitz2d.d3d7
  blitz2d.gl
  blitz2d
  graphics.d3d7
  graphics.gl
  graphics
  ode
  runtime.sdl
  runtime.windows
)

# add all modules to test env
set(MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../modules)
file(GLOB modules RELATIVE ${MODULES_DIR}/bb ${MODULES_DIR}/bb/*)
foreach(module ${modules})
  if(EXISTS ${MODULES_DIR}/bb/${module}/CMakeLists.txt AND NOT ${module} IN_LIST EXCLUDED_MODULES)
    bb_addmodule(${module})
  endif()
endforeach()

bb_end_runtime()
