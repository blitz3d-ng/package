set(BUILD_SHARED_LIBS OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE STRING "" FORCE)
set(GLFW_INSTALL OFF CACHE STRING "" FORCE)

IF (BB_MACOS)
  set(GLFW_USE_CHDIR OFF CACHE STRING "" FORCE)
ENDIF()

add_subdirectory(tree)

target_compile_definitions(glfw PRIVATE GLEW_STATIC)

# solve a strange linker issue with mingw...
IF(BB_MINGW32)
  add_subdirectory(mingw-guid)

  cmake_policy(SET CMP0079 NEW)
  target_include_directories(glfw PRIVATE mingw-guid)
  target_link_libraries(glfw glfw.guid)
ENDIF()

add_library(glfw3 ALIAS glfw)
