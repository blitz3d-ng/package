set(PNG_SHARED OFF CACHE STRING "" FORCE)
set(PNG_BUILD_ZLIB ON CACHE STRING "" FORCE)
set(PNG_DEBUG_POSTFIX "" CACHE STRING "" FORCE)

add_subdirectory(tree)

target_output_to_toolchain(png_static)
set_target_properties(png_static PROPERTIES OUTPUT_NAME png)

add_library(png ALIAS png_static)
