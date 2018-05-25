IF(BB_WINDOWS)
  add_library (bb.filesystem.windows STATIC filesystem.windows.h dir.h dir.cpp driver.cpp driver.h module.cpp)
  target_link_libraries (bb.filesystem.windows bb.filesystem)
ENDIF()
