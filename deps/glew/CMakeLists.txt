optimize_for_size()

if(NOT BB_ANDROID AND NOT BB_NX AND NOT BB_OVR)
  add_library(glew STATIC src/glew.c)
  target_output_to_toolchain(glew)
  target_include_directories(glew PUBLIC include)
  target_compile_options(glew PUBLIC -DGLEW_STATIC)
endif()
