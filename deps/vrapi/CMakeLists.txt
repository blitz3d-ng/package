if(NOT BB_OVR)
  return()
endif()

set(BUILD_TYPE Release)
if(BUILD_TYPE STREQUAL "Debug")
  set(BUILD_TYPE Debug)
endif()

set(VRAPI_SRC_DIR ${OVR_HOME}/VrApi/Libs/Android/${ANDROID_ABI}/${BUILD_TYPE})
set(VRAPI_DEST_DIR ${TOOLCHAIN_PATH}/lib)

if (EXISTS ${VRAPI_SRC_DIR}/libvrapi.so)
  file(COPY ${VRAPI_SRC_DIR}/libvrapi.so DESTINATION ${VRAPI_DEST_DIR})
  add_library(vrapi STATIC IMPORTED GLOBAL)

  set_target_properties(vrapi PROPERTIES IMPORTED_LOCATION ${VRAPI_DEST_DIR}/libvrapi.so)
  target_include_directories(vrapi INTERFACE ${OVR_HOME}/VrApi/Include)
endif()
