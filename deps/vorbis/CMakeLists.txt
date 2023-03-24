IF(NOT BB_WINDOWS)
  # set(Ogg::ogg "s" CACHE STRING "" FORCE)

  file(READ tree/CMakeLists.txt FILE_CONTENTS)
  string(REPLACE "find_package(Ogg" "# find_package(Ogg" FILE_CONTENTS ${FILE_CONTENTS})
  file_write_changes(tree/CMakeLists.txt "${FILE_CONTENTS}")


  add_subdirectory(tree)
  target_output_to_toolchain(vorbis)
  target_output_to_toolchain(vorbisfile)
ENDIF()
