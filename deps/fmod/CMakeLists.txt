if(NOT BB_WINDOWS)
  return()
endif()

add_library(fmod INTERFACE)
target_include_directories(fmod INTERFACE include)
target_link_directories(fmod INTERFACE lib)

file(COPY
  bin/fmod.dll
  bin/fmod64.dll
  DESTINATION ${TOOLCHAIN_BIN_PATH}
)
