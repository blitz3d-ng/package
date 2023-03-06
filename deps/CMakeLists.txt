IF(NOT BB_IOS)
  add_subdirectory(zlib)
ENDIF()

IF(BB_ANDROID)
  add_subdirectory(android_native_app_glue)
ENDIF()

IF(BB_OVR)
  add_subdirectory(vrapi)
ENDIF()

IF(NOT BB_EMSCRIPTEN AND NOT BB_NX)
  add_subdirectory(enet)
ENDIF()

IF(BB_LINUX)
  add_subdirectory(uuid)
ENDIF()

# add_subdirectory(luajit)
add_subdirectory(assimp)
add_subdirectory(crossguid)
add_subdirectory(freetype2)
add_subdirectory(glew)
add_subdirectory(json-hpp)
add_subdirectory(ogg)
add_subdirectory(vorbis)
add_subdirectory(steamworks)
add_subdirectory(toml11)

IF(NOT BB_NX)
  add_subdirectory(sdl)
ENDIF()

IF(BB_DESKTOP OR BB_ANDROID OR BB_OVR)
  add_subdirectory(freeimage)
ENDIF()

IF(BB_DESKTOP AND BB64)
  add_subdirectory(wxwidgets)
ENDIF()

IF(BB_WINDOWS)
  add_subdirectory(dxsdk)
  add_subdirectory(fmod)
ENDIF()

IF((BB_DESKTOP OR BB_EMSCRIPTEN OR BB_NX) AND NOT BB_WOA64)
  add_subdirectory(ode)
ENDIF()
