set(PNG_SHARED OFF CACHE STRING "" FORCE)
set(PNG_BUILD_ZLIB ON CACHE STRING "" FORCE)
set(PNG_DEBUG_POSTFIX "" CACHE STRING "" FORCE)

add_subdirectory(tree)

target_link_libraries(png_static ${ZLIB})
target_include_directories(png_static PUBLIC tree ${CMAKE_CURRENT_BINARY_DIR}/tree)
set_target_properties(png_static PROPERTIES OUTPUT_NAME png)
target_output_to_toolchain(png_static)

add_library(png ALIAS png_static)
