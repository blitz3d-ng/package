add_library(glew STATIC src/glew.c)
target_include_directories(glew PRIVATE include)
target_compile_options(glew PUBLIC -DGLEW_STATIC)
