if(BB_OVR)
  return()
endif()

set(VRAPI_SRC_DIR ${OVR_HOME}/VrApi/Libs/Android/${ANDROID_ABI}/${CMAKE_BUILD_TYPE})
set(VRAPI_DEST_DIR ${PROJECT_BINARY_DIR})

if (EXISTS ${VRAPI_SRC_DIR}/libvrapi.so)
  file(COPY ${VRAPI_SRC_DIR}/libvrapi.so DESTINATION ${VRAPI_DEST_DIR})
  add_library(vrapi STATIC IMPORTED GLOBAL)

  set_target_properties(vrapi PROPERTIES IMPORTED_LOCATION ${VRAPI_DEST_DIR}/libvrapi.so)
  target_include_directories(vrapi INTERFACE ${OVR_HOME}/VrApi/Include)
endif()
