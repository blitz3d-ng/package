if(NOT BB_WIN64)
  return()
endif()

include(FetchContent)
FetchContent_Declare(tiltfive_sdk
  URL https://files.tiltfive.com/tiltfive_sdk_1.3.2.tar.xz
  TLS_VERIFY true
)
FetchContent_Populate(tiltfive_sdk)

set(SDK_PATH ${tiltfive_sdk_SOURCE_DIR}/Native)

add_library(tiltfive INTERFACE)
target_include_directories(tiltfive INTERFACE ${SDK_PATH}/include)
if(BB_WINDOWS)
  target_link_libraries(tiltfive INTERFACE ${SDK_PATH}/lib/win/${BB_ARCH}/TiltFiveNative.dll.if.lib)

  file(COPY
    ${SDK_PATH}/lib/win/${BB_ARCH}/TiltFiveNative.dll
    DESTINATION ${TOOLCHAIN_BIN_PATH}
  )
endif()

