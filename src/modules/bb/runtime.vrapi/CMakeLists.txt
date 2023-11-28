if(TARGET vrapi)
  bb_start_module(runtime.vrapi)
  set(DEPENDS_ON bb.runtime bb.input bb.pixmap bb.blitz3d.gl bb.context.egl bb.system.ndk)
  set(LIBS android_native_app_glue vrapi)
  set(SOURCES runtime.vrapi.h runtime.vrapi.cpp)
  set(EXTRA_FILES Runtime.jar libvrapi.so)
  bb_end_module()

  add_jar(RTjar apk/Game.java
    INCLUDE_JARS ${ANDROID_JAR}
    OUTPUT_NAME Runtime
    OUTPUT_DIR ${TOOLCHAIN_PATH}/lib
  )

  file(INSTALL apk/manifest.template.xml DESTINATION ${TOOLCHAIN_PATH})
endif()
