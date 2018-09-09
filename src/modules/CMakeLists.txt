set (CMAKE_CXX_STANDARD 11)
include_directories (SYSTEM .)

# add all modules in bb/
file(GLOB modules RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} bb/*)
foreach(module ${modules})
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${module}/CMakeLists.txt)
    add_subdirectory(${module})
  ENDIF()
endforeach()
