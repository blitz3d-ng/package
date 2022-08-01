if(TARGET SDL2)
  bb_start_module(runtime.sdl)
  set(DEPENDS_ON bb.runtime bb.input bb.pixmap bb.blitz2d bb.graphics bb.blitz3d.gl bb.timer)
  set(SOURCES runtime.sdl.cpp runtime.sdl.h)

  set(LIBS SDL2)
  if(BB_MOBILE)
    set(LIBS ${LIBS} SDL2main)
  endif()

  if(BB_LINUX)
    set(LIBS ${LIBS} dl)
  elseif(BB_MACOS)
    set(SYSTEM_LIBS "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation" "-framework CoreVideo" "-framework CoreAudio" "-framework ForceFeedback" "-framework AudioToolBox" "-framework Metal" "-framework GameController" "-framework CoreHaptics" "-framework Carbon" iconv)
  elseif(BB_WINDOWS)
    set(LIBS ${LIBS} dxguid)
  elseif(BB_IOS)
    set(SYSTEM_LIBS "-framework Foundation" "-framework UIKit" "-framework CoreFoundation" "-framework CoreVideo" "-framework CoreAudio" "-framework AVFAudio" "-framework AudioToolBox" "-framework Metal" "-framework GameController" "-framework CoreHaptics" "-framework CoreBluetooth" "-framework CoreMotion" "-framework CoreGraphics" "-framework QuartzCore" iconv)
  elseif(BB_ANDROID)
    set(SYSTEM_LIBS GLESv1_CM GLESv2 OpenSLES android dl)
  endif()
  bb_end_module()

  if(BB_ANDROID)
    add_jar(RTjar android/Game.java
      INCLUDE_JARS ${ANDROID_JAR}
                   ${TOOLCHAIN_PATH}/lib/SDL.jar
      OUTPUT_NAME Runtime
      OUTPUT_DIR ${TOOLCHAIN_PATH}/lib
    )

    file(INSTALL android/manifest.template.xml DESTINATION ${TOOLCHAIN_PATH})
  endif()
endif()
