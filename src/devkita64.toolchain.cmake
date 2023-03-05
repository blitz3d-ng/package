set(CMAKE_SYSTEM_NAME Generic)

set(DEVKITPRO true)

set(ARCH aarch64)
set(TRIPLE ${ARCH}-none-elf)

set(CMAKE_C_COMPILER   ${TRIPLE}-gcc)
set(CMAKE_CXX_COMPILER ${TRIPLE}-g++)
set(CMAKE_RC_COMPILER  ${TRIPLE}-windres)

set(CMAKE_EXE_LINKER_FLAGS " -static")
set(CMAKE_SHARED_LIBRARY_PREFIX_CXX "")

set(CMAKE_FIND_ROOT_PATH /opt/devkitpro/devkitA64)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

include_directories(/opt/devkitpro/libnx)
include_directories(/opt/devkitpro/portlibs/switch/include)
