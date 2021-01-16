set (CMAKE_CXX_STANDARD 11)

set(SOURCES stdutil.h stdutil.cpp)

if(BB_ANDROID OR BB_NX)
  set(SOURCES ${SOURCES} ecvt.c gcvt.c)
endif()

add_library(stdutil STATIC ${SOURCES})
