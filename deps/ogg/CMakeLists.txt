IF(NOT BB_WINDOWS)
  add_subdirectory(tree)
  target_output_to_toolchain(ogg)
ENDIF()
