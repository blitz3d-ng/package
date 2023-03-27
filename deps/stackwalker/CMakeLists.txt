if(NOT MSVC)
  return()
endif()

file(READ tree/CMakeLists.txt FILE_CONTENTS)
string(REPLACE "$<BUILD_INTERFACE:\${CMAKE_SOURCE_DIR}" "$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}" FILE_CONTENTS "${FILE_CONTENTS}")
file_write_changes(tree/CMakeLists.txt "${FILE_CONTENTS}")

add_subdirectory(tree)
