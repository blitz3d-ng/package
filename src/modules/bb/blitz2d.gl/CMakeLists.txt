set(MODULES bb.blitz bb.pixmap bb.blitz2d)
set(LIBS glew)

IF(BB_WINDOWS)
  set(LIBS ${LIBS} opengl32)
ENDIF()

add_library (bb.blitz2d.gl STATIC blitz2d.gl.cpp blitz2d.gl.h)
target_link_libraries (bb.blitz2d.gl ${MODULES} ${LIBS})
