bb_start_runtime(test Test bbCreateConsoleRuntime)

set(EXCLUDED_MODULES
  runtime.sdl

  blitz3d.d3d7
  blitz2d.d3d7
  graphics.d3d7
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
