if(NOT BB_DESKTOP AND NOT BB_NX)
  return()
endif()

# patch to make it more friendly
file(READ tree/CMakeLists.txt FILE_CONTENTS)
string(REPLACE "add_custom_target(uninstall" "# add_custom_target(uninstall" FILE_CONTENTS "${FILE_CONTENTS}")
file_write_changes(tree/CMakeLists.txt "${FILE_CONTENTS}")

# for mingw
file(READ tree/ode/src/threading_pool_win.cpp FILE_CONTENTS)
string(REPLACE "#include <Windows.h>" "#include <windows.h>" FILE_CONTENTS "${FILE_CONTENTS}")
file_write_changes(tree/ode/src/threading_pool_win.cpp "${FILE_CONTENTS}")

set(BUILD_SHARED_LIBS OFF CACHE STRING "" FORCE)
set(ODE_WITH_DEMOS OFF CACHE STRING "" FORCE)
set(ODE_WITH_TESTS OFF CACHE STRING "" FORCE)
set(ODE_DOUBLE_PRECISION OFF CACHE STRING "" FORCE)

add_subdirectory(tree)
add_library(ode ALIAS ODE)
target_output_to_toolchain(ODE)
set_target_properties(ODE PROPERTIES UNITY_BUILD false)

# temporarily silence warnings
if (NOT BB_MSVC)
  target_compile_options(ODE PRIVATE -Wno-deprecated-declarations)
  target_compile_options(ODE PRIVATE -Wno-undefined-var-template)
endif()

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

if(BB_NX)
  target_compile_definitions(ODE PRIVATE __linux__)
endif()
