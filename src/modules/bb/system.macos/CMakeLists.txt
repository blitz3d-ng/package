if(BB_MACOS)
  bb_start_module(system.macos)
  set(DEPENDS_ON bb.runtime bb.system)
  set(SOURCES system.macos.cpp system.macos.h lookup_font_file.m)
  set(SYSTEM_LIBS "-framework Cocoa")
  bb_end_module()

  set_target_properties(bb.system.macos PROPERTIES UNITY_BUILD_MODE GROUP)
endif()
