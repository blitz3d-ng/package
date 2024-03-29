bin2h(SOURCE_FILE default.glsl HEADER_FILE default.glsl.h VARIABLE_NAME DEFAULT_GLSL)

bb_start_module(graphics.gl)
set(DEPENDS_ON bb.graphics)
set(SOURCES graphics.gl.cpp graphics.gl.h canvas.cpp canvas.h default.glsl.h)

IF(BB_WINDOWS)
  set(SYSTEM_LIBS opengl32)
ELSEIF(BB_MACOS)
  set(SYSTEM_LIBS "-framework OpenGL")
ELSEIF(BB_LINUX)
  set(SYSTEM_LIBS GL X11 Xrandr Xinerama Xcursor Xxf86vm pthread)
ELSEIF(BB_IOS)
  set(SYSTEM_LIBS "-framework OpenGLES")
ENDIF()

IF(BB_DESKTOP)
  set(LIBS ${LIBS} glew)
ENDIF()

bb_end_module()

if(BB_MACOS)
  target_compile_definitions(bb.graphics.gl PUBLIC -DGL_SILENCE_DEPRECATION)
elseif(BB_IOS)
  target_compile_definitions(bb.graphics.gl PUBLIC -DGLES_SILENCE_DEPRECATION)
elseif(BB_NX)
  target_include_directories(bb.graphics.gl PUBLIC /opt/devkitpro/portlibs/switch/include)
  target_link_directories(bb.graphics.gl PUBLIC /opt/devkitpro/portlibs/switch/lib)
endif()
