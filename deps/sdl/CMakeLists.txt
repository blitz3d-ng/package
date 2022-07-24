set(SDL_SHARED OFF CACHE BOOLEAN "" FORCE)
set(SDL_STATIC ON CACHE BOOLEAN "" FORCE)
set(SDL_STATIC_PIC ON CACHE BOOLEAN "" FORCE)

add_subdirectory(tree)

target_output_to_toolchain(SDL2-static)
add_library(SDL2 ALIAS SDL2-static)
