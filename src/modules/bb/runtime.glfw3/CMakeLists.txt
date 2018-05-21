add_library (bb.runtime.glfw3 STATIC runtime.glfw3.cpp runtime.glfw3.h scancodes.cpp)
target_include_directories (bb.runtime.glfw3 PRIVATE SYSTEM ../../../deps/glew/include ../../../deps/glfw3/src/include ../../../deps/freetype2/src/include)
target_link_libraries (bb.runtime.glfw3 bb.blitz bb.runtime bb.system bb.input bb.pixmap bb.blitz2d bb.graphics bb.blitz3d bb.blitz2d.gl bb.blitz3d.gl bb.timer glfw)

IF(BB_MACOS)
  target_link_libraries (bb.runtime.glfw3 "-framework OpenGL")
ENDIF()
