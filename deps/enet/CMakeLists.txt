if (BB_EMSCRIPTEN OR BB_NX)
  return()
endif()

set(CMAKE_UNITY_BUILD ${ENABLE_UNITY_BUILD})

add_subdirectory(src)
target_output_to_toolchain(enet)
target_include_directories (enet INTERFACE src/include)
