set(GLFM_BUILD_EXAMPLE OFF)
add_subdirectory(src)

set_target_properties(glfm PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/_release/toolchains/${BB_PLATFORM}/lib/${BB_ENV}
)
