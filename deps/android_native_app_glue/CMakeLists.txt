if(NOT BB_NDK)
  return()
endif()

add_library(android_native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories(android_native_app_glue PUBLIC ${ANDROID_NDK}/sources/android/native_app_glue)
target_link_libraries(android_native_app_glue android)
