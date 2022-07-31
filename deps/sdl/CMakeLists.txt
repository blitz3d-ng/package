set(SDL_SHARED OFF CACHE BOOLEAN "" FORCE)
set(SDL_STATIC ON CACHE BOOLEAN "" FORCE)
set(SDL_STATIC_PIC ON CACHE BOOLEAN "" FORCE)
if(BB_LINUX)
  set(SDL_OPENGLES OFF CACHE BOOLEAN "" FORCE)
endif()

add_subdirectory(tree)

target_output_to_toolchain(SDL2-static)
if(BB_MOBILE)
  target_output_to_toolchain(SDL2main)
endif()
add_library(SDL2 ALIAS SDL2-static)
