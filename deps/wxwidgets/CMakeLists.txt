set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(wxBUILD_SHARED OFF CACHE STRING "" FORCE)
set(wxBUILD_USE_STATIC_RUNTIME ON CACHE STRING "" FORCE)

# TODO: find out why this suddenly started happening...
if(BB_LINUX)
  add_definitions(-DHAVE_DLOPEN)
endif()

add_subdirectory(tree)
