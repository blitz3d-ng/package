if(NOT BB_DESKTOP)
  return()
endif()

# patch to make it more friendly
file(READ tree/CMakeLists.txt FILE_CONTENTS)
string(REPLACE "add_custom_target(uninstall" "# add_custom_target(uninstall" FILE_CONTENTS "${FILE_CONTENTS}")
file_write_changes(tree/CMakeLists.txt "${FILE_CONTENTS}")

set(BUILD_SHARED_LIBS OFF CACHE STRING "" FORCE)
set(ODE_WITH_DEMOS OFF CACHE STRING "" FORCE)
set(ODE_WITH_TESTS OFF CACHE STRING "" FORCE)
set(ODE_DOUBLE_PRECISION OFF CACHE STRING "" FORCE)

add_subdirectory(tree)

if(WIN32)
  set_target_properties(
    ODE
    PROPERTIES
    OUTPUT_NAME ODE
    DEBUG_POSTFIX ""
    MINSIZEREL_POSTFIX ""
    RELEASE_POSTFIX ""
    RELWITHDEBINFO_POSTFIX ""
  )
endif()

target_output_to_toolchain(ODE)
