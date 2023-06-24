if (BB_EMSCRIPTEN OR BB_NX)
  return()
endif()

add_subdirectory(src)
target_output_to_toolchain(enet)
target_include_directories (enet INTERFACE src/include)
