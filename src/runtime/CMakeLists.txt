# some useful macros...
macro(bb_addmodule name)
  if(TARGET bb.${name})
    set(LIBS ${LIBS} bb.${name})
    set(DEPENDS_ON ${DEPENDS_ON} ${name})
  endif()
endmacro()

macro(bb_runtime id)
  set(name runtime.${id})

  # static
  add_library(${name}.static STATIC ${SOURCES})
  target_link_libraries(${name}.static ${LIBS} ${SYSTEM_LIBS} bb.stub)

  set(IFACE ${BUILD_PATH}/${name}.i)
  file(WRITE  ${IFACE} "ID: ${id}\n")
  file(APPEND ${IFACE} "DEPS: ${DEPENDS_ON}\n")

  if(BB_COVERAGE)
    set_property(TARGET ${name}.static APPEND_STRING PROPERTY COMPILE_FLAGS " ${BB_COVERAGE}")
  endif()

  # shared
  if(BB_WINDOWS)
    set(SOURCES ${SOURCES} ../../legacy/bbruntime_dll/bbruntime_dll.rc ../../legacy/bbruntime_dll/dpi.manifest)
  endif()

  add_library(${name}.shared SHARED ${SOURCES})
  target_link_libraries(${name}.shared bbruntime_dll ${name}.static)

  if(BB_WINDOWS)
    target_output_to_bin(${name}.shared)

    if(BB32)
      set_target_properties(${name}.shared PROPERTIES OUTPUT_NAME "runtime.${id}")
    else()
      set_target_properties(${name}.shared PROPERTIES OUTPUT_NAME "runtime64.${id}")
    endif()

    if(BB_MSVC)
      if(BB_WIN32)
        set_property(TARGET ${name}.shared APPEND_STRING PROPERTY LINK_FLAGS " /include:_runtimeGetRuntime")
      else()
        set_property(TARGET ${name}.shared APPEND_STRING PROPERTY LINK_FLAGS " /include:runtimeGetRuntime")
      endif()
      if(NOT BB_DEBUG)
        set_property(TARGET ${name}.shared APPEND_STRING PROPERTY LINK_FLAGS " /subsystem:windows")
      endif()
    else()
      # TODO: figure this out...
      set_property(TARGET ${name}.shared APPEND_STRING PROPERTY LINK_FLAGS " -static")
    endif()
  else()
    set_target_properties(${name}.shared
      PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY ${TOOLCHAIN_PATH}/lib/${BB_TRIPLE}
      OUTPUT_NAME "runtime.${id}"
    )
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
