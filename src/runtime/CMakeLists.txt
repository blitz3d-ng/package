set (CMAKE_CXX_STANDARD 11)

# some useful macros...
macro(bb_addmodule name)
  if(TARGET ${name})
    set(DEPENDS_ON ${DEPENDS_ON} ${name})
  endif()
endmacro()

macro(bb_runtime name)
  # static
  add_library(${name}.static STATIC ${SOURCES})
  target_link_libraries(${name}.static ${DEPENDS_ON} ${SYSTEM_LIBS})

  if(BB_COVERAGE)
    set_property(TARGET ${name}.static APPEND_STRING PROPERTY COMPILE_FLAGS " ${BB_COVERAGE}")
  endif()

  # shared
  if(BB_WINDOWS)
    set(SOURCES ${SOURCES} ../../../bbruntime_dll/bbruntime_dll.rc)
  endif()

  add_library(${name}.shared SHARED ${SOURCES})

  if(BB_WINDOWS)
    target_link_libraries(${name}.shared bbruntime_dll ${name}.static)
    set_target_properties(${name}.shared PROPERTIES OUTPUT_NAME "${name}")
    if(BB_MSVC)
      target_link_options(${name}.shared PRIVATE /include:_runtimeGetRuntime /subsystem:windows)
    else()
      set_property(TARGET ${name}.shared APPEND_STRING PROPERTY LINK_FLAGS " -static")
    endif()
  else()
    target_link_libraries(${name}.shared ${name}.static)
  endif()

  if(BB_COVERAGE)
    set_property(TARGET ${name}.shared APPEND_STRING PROPERTY COMPILE_FLAGS " ${BB_COVERAGE}")
    set_property(TARGET ${name}.shared APPEND_STRING PROPERTY LINK_FLAGS " ${BB_COVERAGE}")
  endif()
endmacro()

# bring in all the runtimes
file(GLOB runtimes RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
foreach(runtime ${runtimes})
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${runtime}/CMakeLists.txt)
    add_subdirectory(${runtime})
  ENDIF()
endforeach()
