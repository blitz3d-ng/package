IF(NOT BB_EMSCRIPTEN)
  bb_start_module(unit-test)
  set(DEPENDS_ON bb.blitz)
  set(SOURCES unit-test.cpp unit-test.h module.link.cpp)
  bb_end_module()

  # generate a unit_test userlib for legacy Blitz3D.
  if(BB_WINDOWS)
    add_library(unit_test SHARED unit-test.cpp unit-test.h userlib.cpp)
    target_link_libraries(unit_test bb.blitz bb.hook)
    target_compile_definitions(unit_test PRIVATE USERLIB)
    set_property(TARGET unit_test APPEND_STRING PROPERTY LINK_FLAGS " -static")
    set_target_properties(unit_test
      PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY ${RELEASE_PATH}/userlibs
    )

    if(BB64)
      set_target_properties(unit_test PROPERTIES OUTPUT_NAME "unit_test64")
    endif()
  endif()
ENDIF()
