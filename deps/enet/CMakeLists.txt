if (BB_EMSCRIPTEN OR BB_NX)
  return()
endif()

add_subdirectory(tree)
target_output_to_toolchain(enet)
target_include_directories (enet INTERFACE tree/include)
