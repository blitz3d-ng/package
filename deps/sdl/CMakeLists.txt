if(BB_OVR OR BB_NX)
  return()
endif()

add_compile_definitions(SDL_LEAN_AND_MEAN)
add_compile_definitions(GL_SILENCE_DEPRECATION GLES_SILENCE_DEPRECATION)

# no need...
file(READ tree/CMakeLists.txt FILE_CONTENTS)
string(REPLACE "\"SDL2-static\"" "\"SDL2\"" FILE_CONTENTS "${FILE_CONTENTS}")
file_write_changes(tree/CMakeLists.txt "${FILE_CONTENTS}")

set(SDL_SHARED OFF CACHE STRING "" FORCE)
set(SDL_STATIC ON CACHE STRING "" FORCE)
set(SDL_STATIC_PIC ON CACHE STRING "" FORCE)
set(SDL_FORCE_STATIC_VCRT ON CACHE STRING "" FORCE)
set(SDL_LIBC ON CACHE STRING "" FORCE)
set(SDL_CMAKE_DEBUG_POSTFIX "" CACHE STRING "" FORCE)
if(BB_LINUX)
  set(SDL_OPENGLES OFF CACHE BOOLEAN "" FORCE)
endif()

add_subdirectory(tree)

target_output_to_toolchain(SDL2-static)
if(BB_MOBILE)
  target_output_to_toolchain(SDL2main)
endif()
add_library(SDL2 ALIAS SDL2-static)

if(BB_ANDROID)
  set(JAVA_SRC tree/android-project/app/src/main/java/org/libsdl/app)

  add_jar(SDLjar
    ${JAVA_SRC}/HIDDevice.java
    ${JAVA_SRC}/HIDDeviceBLESteamController.java
    ${JAVA_SRC}/HIDDeviceManager.java
    ${JAVA_SRC}/HIDDeviceUSB.java
    ${JAVA_SRC}/SDL.java
    ${JAVA_SRC}/SDLActivity.java
    ${JAVA_SRC}/SDLAudioManager.java
    ${JAVA_SRC}/SDLControllerManager.java
    ${JAVA_SRC}/SDLSurface.java
    INCLUDE_JARS ${ANDROID_JAR}
    OUTPUT_NAME SDL
    OUTPUT_DIR ${TOOLCHAIN_PATH}/lib
  )
endif()
