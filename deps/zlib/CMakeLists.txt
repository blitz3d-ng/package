if(BB_IOS)
  return()
endif()

if (BB_MINGW)
  set(MINGW true)
endif()

add_subdirectory(tree)

target_output_to_toolchain(zlibstatic)
set_target_properties(zlibstatic PROPERTIES
  OUTPUT_NAME zlibstatic
  UNITY_BUILD false
)

add_library(ZLIB::ZLIB ALIAS zlibstatic)
