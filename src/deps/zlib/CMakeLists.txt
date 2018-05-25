add_definitions(-DZLIB_DEBUG)
add_subdirectory(src)
target_include_directories(zlib PUBLIC src ${CMAKE_BINARY_DIR}/src/deps/zlib/src)
