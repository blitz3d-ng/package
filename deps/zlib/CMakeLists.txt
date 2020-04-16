if (BB_MINGW32)
  set(MINGW true)
endif()

set(BUILD_SHARED_LIBS off)

add_subdirectory(src)
target_include_directories(zlib PUBLIC src ${CMAKE_BINARY_DIR}/src/deps/zlib/src)
target_include_directories(zlibstatic PUBLIC src ${CMAKE_BINARY_DIR}/src/deps/zlib/src)

set_target_properties(zlibstatic PROPERTIES OUTPUT_NAME zlibstatic)

# disable these since the CMakeLists.txt does not offer a flag.
set_target_properties(zlib PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
set_target_properties(example PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
set_target_properties(minigzip PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
