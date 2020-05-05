set (CMAKE_CXX_STANDARD 11)

set (CMAKE_EXECUTABLE_SUFFIX_ )

add_library(linker SHARED main.cpp linker.h linker.cpp image_util.h image_util.cpp)
target_include_directories (linker PRIVATE ../../modules)
if(BB_MINGW32)
  set_property(TARGET linker APPEND_STRING PROPERTY LINK_FLAGS " -static")
endif()
target_link_libraries(linker stdutil)
set_target_properties(linker PROPERTIES LINK_FLAGS "/subsystem:windows")
