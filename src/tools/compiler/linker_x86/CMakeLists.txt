set (CMAKE_EXECUTABLE_SUFFIX_ )

add_library(linker SHARED main.cpp linker.h linker.cpp image_util.h image_util.cpp)
target_include_directories (linker PRIVATE ../../modules)
target_link_libraries(linker stdutil)

target_output_to_bin(linker)

set_target_properties(linker PROPERTIES PREFIX "")

if(BB_WINDOWS)
  if(BB_MINGW)
    set_property(TARGET linker APPEND_STRING PROPERTY LINK_FLAGS " -static")
  else()
    set_target_properties(linker PROPERTIES LINK_FLAGS "/subsystem:windows")
  endif()

  if(BB64)
    set_target_properties(linker PROPERTIES OUTPUT_NAME "linker64")
  endif()
endif()
