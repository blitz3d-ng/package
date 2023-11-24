if(BB_IOS)
  return()
endif()

if (BB_MINGW)
  set(MINGW true)
endif()

set(BUILD_SHARED_LIBS off)

add_subdirectory(tree)

target_include_directories(zlibstatic PUBLIC tree ${CMAKE_BINARY_DIR}/deps/zlib/tree)

# silence zlib warnings for the time being
if(NOT BB_MSVC)
  target_compile_options(zlibstatic PRIVATE -Wno-deprecated-non-prototype)
endif()

target_output_to_toolchain(zlibstatic)
set_target_properties(zlibstatic PROPERTIES
  OUTPUT_NAME zlibstatic
  UNITY_BUILD false
)

add_library(ZLIB::ZLIB ALIAS zlibstatic)
