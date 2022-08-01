set(SDL_SHARED OFF CACHE BOOLEAN "" FORCE)
set(SDL_STATIC ON CACHE BOOLEAN "" FORCE)
set(SDL_STATIC_PIC ON CACHE BOOLEAN "" FORCE)
set(SDL_FORCE_STATIC_VCRT ON CACHE BOOLEAN "" FORCE)
set(SDL_LIBC ON CACHE BOOLEAN "" FORCE)
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
    INCLUDE_JARS ${ANDROID_JAR}
    OUTPUT_NAME SDL
    OUTPUT_DIR ${TOOLCHAIN_PATH}/lib
  )
endif()
