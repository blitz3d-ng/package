set(CMAKE_UNITY_BUILD false)

set(BUILD_SHARED_LIBS OFF CACHE STRING "" FORCE)

set(DISABLE_FORCE_DEBUG_POSTFIX ON CACHE STRING "" FORCE)

set(FT_DISABLE_ZLIB TRUE CACHE STRING "" FORCE)
set(ZLIB_FOUND 1 CACHE STRING "" FORCE)

set(PNG_FOUND 1 CACHE STRING "" FORCE)

# TODO: probably should look into getting these enabled
set(FT_DISABLE_HARFBUZZ TRUE CACHE STRING "" FORCE)
set(FT_DISABLE_BZIP2    TRUE CACHE STRING "" FORCE)
set(FT_DISABLE_BROTLI   TRUE CACHE STRING "" FORCE)

add_subdirectory(tree)

target_link_libraries(freetype PUBLIC png_static ${ZLIB})
target_output_to_toolchain(freetype)
