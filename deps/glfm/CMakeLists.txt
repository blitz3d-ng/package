# set(GLFM_BUILD_EXAMPLE OFF)
# add_subdirectory(src)

if(APPLE)
  enable_language(OBJC)
endif()

add_library(glfm src/src/glfm_platform.h src/src/glfm_platform_ios.m)
target_include_directories(glfm PUBLIC src/include)
target_include_directories(glfm PRIVATE src/src)

# set_target_properties(glfm PROPERTIES
#   ARCHIVE_OUTPUT_DIRECTORY ${BUILD_PATH}/lib/${BB_ENV}
# )
