if (BB_MINGW32)
  set(MINGW true)
endif()

add_subdirectory(src)
target_include_directories(zlib PUBLIC src ${CMAKE_BINARY_DIR}/src/deps/zlib/src)
target_include_directories(zlibstatic PUBLIC src ${CMAKE_BINARY_DIR}/src/deps/zlib/src)

set_target_properties(zlibstatic PROPERTIES OUTPUT_NAME zlibstatic)
