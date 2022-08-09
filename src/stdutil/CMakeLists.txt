set(SOURCES stdutil.h stdutil.cpp)

if(BB_ANDROID OR BB_NX OR BB_OVR)
  set(SOURCES ${SOURCES} ecvt.c gcvt.c)
endif()

add_library(stdutil STATIC ${SOURCES})
target_output_to_toolchain(stdutil)
