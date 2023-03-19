IF(BB_WINDOWS)
  bb_start_module(multiplay.directplay)
  set(DEPENDS_ON bb.runtime)
  set(SOURCES multiplay.cpp multiplay.h multiplay_setup.cpp multiplay_setup.h multiplay_setup.rc)
  bb_end_module()

  target_include_directories(bb.multiplay PRIVATE ../../../../common/include)
  target_link_libraries(bb.multiplay dxsdk)
ENDIF()
