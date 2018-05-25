set(GLFW_BUILD_EXAMPLES false)
set(GLFW_BUILD_TESTS false)
set(GLFW_BUILD_DOCS false)
set(GLFW_INSTALL false)

IF (BB_MACOS)
  set(GLFW_USE_CHDIR false)
ENDIF()

add_definitions(-DGLEW_STATIC)
add_subdirectory(src)
