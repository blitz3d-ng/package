# the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)

set(MINGW true)

if(NOT ARCH)
  set(ARCH i686)
endif()

set(CMAKE_C_COMPILER ${ARCH}-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER ${ARCH}-w64-mingw32-g++)
set(CMAKE_RC_COMPILER ${ARCH}-w64-mingw32-windres)

#set(CMAKE_EXE_LINKER_FLAGS " -static") # TODO: this should be turned on
set(CMAKE_SHARED_LIBRARY_PREFIX_CXX "")

set(CMAKE_FIND_ROOT_PATH /usr/bin/${ARCH}-w64-mingw32)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
