include(ExternalProject)

set(UUID_ROOT ${CMAKE_CURRENT_BINARY_DIR}/root)
set(UUID_INCLUDE ${UUID_ROOT}/include)
set(UUID_LIB_DIR ${UUID_ROOT}/lib)
set(UUID_LIB ${UUID_LIB_DIR}/libuuid.a)

if(NOT EXISTS ${UUID_LIB})
  file(MAKE_DIRECTORY ${UUID_LIB_DIR})
  file(TOUCH ${UUID_LIB})
endif()

ExternalProject_Add(UUID_EXTERNAL
  GIT_REPOSITORY https://git.kernel.org/pub/scm/fs/ext2/e2fsprogs.git
  GIT_TAG v1.46.5
  CONFIGURE_COMMAND ../UUID_EXTERNAL/configure --prefix=${UUID_ROOT} CFLAGS=-fPIC --enable-libuuid
  BUILD_COMMAND cd lib/uuid && make
  INSTALL_COMMAND cd lib/uuid && make install
  UPDATE_DISCONNECTED 1
)

add_custom_command(TARGET UUID_EXTERNAL POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${UUID_LIB} ${BUILD_PATH}/lib
)

add_library(uuid INTERFACE)
add_dependencies(uuid UUID_EXTERNAL)
target_output_to_toolchain(uuid)
target_include_directories(uuid INTERFACE ${UUID_INCLUDE})
target_link_libraries(uuid INTERFACE ${UUID_LIB})
