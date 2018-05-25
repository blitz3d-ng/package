IF(BB_WINDOWS)
  add_library (bb.timer.windows STATIC timer.windows.cpp timer.windows.h)
  target_link_libraries (bb.timer.windows bb.timer)
ENDIF()
