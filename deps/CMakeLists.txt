set(BUILD_SHARED_LIBS OFF CACHE STRING "" FORCE)
set(SKIP_INSTALL_ALL ON CACHE STRING "" FORCE)

# force local zlib
set(ZLIB_FOUND 1 CACHE STRING "" FORCE)

# core
add_subdirectory(zlib)
add_subdirectory(stackwalker)
add_subdirectory(utf8.h)

# ide
add_subdirectory(wxwidgets)

# compiler
add_subdirectory(json-hpp)
add_subdirectory(toml11)

# system
add_subdirectory(android_native_app_glue)
add_subdirectory(vrapi)
add_subdirectory(libnx)

# graphics
add_subdirectory(freetype2)
add_subdirectory(glew)
add_subdirectory(sdl)
add_subdirectory(libpng)
add_subdirectory(freeimage)
add_subdirectory(dxsdk)

# audio
add_subdirectory(fmod)
add_subdirectory(ogg)
add_subdirectory(vorbis)
add_subdirectory(minimp3)

# misc
add_subdirectory(enet)
add_subdirectory(uuid)
add_subdirectory(crossguid)

# third-party wrappers
add_subdirectory(ode)
add_subdirectory(steamworks)
add_subdirectory(tiltfive)
#add_subdirectory(luajit)
