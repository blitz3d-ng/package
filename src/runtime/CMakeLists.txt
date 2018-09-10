set (CMAKE_CXX_STANDARD 11)

# some useful macros...
macro(bb_addmodule name)
  if(TARGET ${name})
    set(DEPENDS_ON ${DEPENDS_ON} ${name})
  endif()
endmacro()

macro(bb_runtime name)
  # static
  add_library(${name} STATIC ${SOURCES})
  target_link_libraries(${name} ${DEPENDS_ON} ${SYSTEM_LIBS})

  if(BB_COVERAGE)
    set_property(TARGET ${name} APPEND_STRING PROPERTY COMPILE_FLAGS " ${BB_COVERAGE}")
  endif()

  # shared
  add_library(${name}.bin SHARED ${SOURCES})
  target_link_libraries(${name}.bin ${name})

  if(BB_WINDOWS)
    set_target_properties(${name}.bin PROPERTIES OUTPUT_NAME "${name}")
    set_property(TARGET ${name}.bin APPEND_STRING PROPERTY LINK_FLAGS " -static")
  endif()

  if(BB_COVERAGE)
    set_property(TARGET ${name}.bin APPEND_STRING PROPERTY COMPILE_FLAGS " ${BB_COVERAGE}")
    set_property(TARGET ${name}.bin APPEND_STRING PROPERTY LINK_FLAGS " ${BB_COVERAGE}")
  endif()
endmacro()

# bring in all the runtimes
file(GLOB runtimes RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
foreach(runtime ${runtimes})
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${runtime}/CMakeLists.txt)
    add_subdirectory(${runtime})
  ENDIF()
endforeach()
