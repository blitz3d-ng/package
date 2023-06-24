if(BB_IOS)
  return()
endif()

if (BB_MINGW)
  set(MINGW true)
endif()

set(BUILD_SHARED_LIBS off)

add_subdirectory(tree)

target_include_directories(zlibstatic PUBLIC tree ${CMAKE_BINARY_DIR}/deps/zlib/tree)
target_output_to_toolchain(zlibstatic)
set_target_properties(zlibstatic PROPERTIES
  OUTPUT_NAME zlibstatic
  UNITY_BUILD false
)

# disable these since the CMakeLists.txt does not offer a flag.
set_target_properties(zlib PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
set_target_properties(example PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
set_target_properties(minigzip PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
IF(TARGET example64)
  set_target_properties(example64 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
ENDIF()
IF(TARGET minigzip64)
  set_target_properties(minigzip64 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
ENDIF()

add_library(ZLIB::ZLIB ALIAS zlibstatic)
