if(NOT BB_WINDOWS)
  return()
endif()

add_library(dxsdk INTERFACE)
target_include_directories(dxsdk INTERFACE include)
if(BB_WIN32)
  target_link_directories(dxsdk INTERFACE lib/x86)
endif()
