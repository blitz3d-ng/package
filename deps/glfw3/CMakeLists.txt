set(GLFW_BUILD_EXAMPLES off)
set(GLFW_BUILD_TESTS off)
set(GLFW_BUILD_DOCS off)
set(GLFW_INSTALL off)

IF (BB_MACOS)
  set(GLFW_USE_CHDIR off)
ENDIF()

add_subdirectory(src)
target_compile_definitions(glfw PRIVATE GLEW_STATIC)
