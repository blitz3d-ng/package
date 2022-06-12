include_directories (SYSTEM .)

# some useful macros...
macro(bb_start_module name)
  set(MODULE_ID ${name})
  set(DEPENDS_ON )
  set(SOURCES )
  set(LIBS )
endmacro()

macro(bb_end_module)
  add_library (bb.${MODULE_ID} STATIC ${SOURCES})
  target_link_libraries (bb.${MODULE_ID} ${DEPENDS_ON} ${LIBS})

  if(BB_COVERAGE)
    set_target_properties(bb.${MODULE_ID} PROPERTIES COMPILE_FLAGS ${BB_COVERAGE})
  endif()

  file(WRITE  ${BUILD_PATH}/cfg/${MODULE_ID}.${BB_ENV}.i "ID: bb.${MODULE_ID}\n")
  file(APPEND ${BUILD_PATH}/cfg/${MODULE_ID}.${BB_ENV}.i "MODULES: ${DEPENDS_ON}\n")
  file(APPEND ${BUILD_PATH}/cfg/${MODULE_ID}.${BB_ENV}.i "LIBS: ${LIBS}\n")
endmacro()

# add all modules in bb/
file(GLOB modules RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} bb/*)
foreach(module ${modules})
  # if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${module}/CMakeLists.txt)
    add_subdirectory(${module})
  # ENDIF()
endforeach()
