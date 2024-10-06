if(NOT BB_DESKTOP OR NOT BB64)
  return()
endif()

set(CMAKE_UNITY_BUILD false)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${ENABLE_LTO})

set(wxBUILD_SHARED OFF CACHE STRING "" FORCE)
set(wxBUILD_USE_STATIC_RUNTIME ON CACHE STRING "" FORCE)

if(BB_LINUX)
  # TODO: find out why this suddenly started happening...
  add_definitions(-DHAVE_DLOPEN)

  # needed to find X11 includes/libs
  set(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH ON)
endif()

add_subdirectory(tree)
