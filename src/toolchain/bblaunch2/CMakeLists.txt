# TODO: support windows + linux
if(NOT BB_MACOS)
  return()
endif()

set(ICON ${CMAKE_CURRENT_SOURCE_DIR}/bblaunch.icns)
set_source_files_properties(${ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(bblaunch2 main.cpp ${ICON})

target_output_directory(bblaunch2 ${RELEASE_PATH})

if(BB_MACOS)
  set_target_properties(bblaunch2 PROPERTIES
    MACOSX_BUNDLE true
    MACOSX_BUNDLE_BUNDLE_NAME Blitz3D
    MACOSX_BUNDLE_BUNDLE_VERSION "0.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.0"
    MACOSX_BUNDLE_ICON_FILE bblaunch.icns
    OUTPUT_NAME "BlitzNG"
  )
endif()
