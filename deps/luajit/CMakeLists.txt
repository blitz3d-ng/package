if(BB_DESKTOP AND NOT BB_MINGW32)
  include(ExternalProject)
  set(LUAJIT_ROOT ${CMAKE_CURRENT_BINARY_DIR}/root)

  if(BB_MACOS OR BB_LINUX)
    set(LUAJIT_INCLUDE ${LUAJIT_ROOT}/include/luajit-2.1)
    set(LUAJIT_LIB_DIR ${LUAJIT_ROOT}/lib)
    set(LUAJIT_LIB ${LUAJIT_LIB_DIR}/libluajit-5.1.a)

    if(BB_MACOS)
      set(MAKE_FLAGS MACOSX_DEPLOYMENT_TARGET=10.14)
      set(MACOS_FIX '') # sed on macos works a bit differently than the GNU one
    endif()

    ExternalProject_Add(LUAJIT_EXTERNAL
      GIT_REPOSITORY https://luajit.org/git/luajit-2.0.git
      GIT_TAG v2.1.0-beta3
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND /bin/sh -c "file ${LUAJIT_LIB} | grep archive > /dev/null || (cp -r ../LUAJIT_EXTERNAL/* . && sed -i ${MACOS_FIX} \"s|/usr/local|${LUAJIT_ROOT}|g\" Makefile && make ${MAKE_FLAGS} install)"
    )
  elseif(BB_WINDOWS)
    ExternalProject_Add(LUAJIT_EXTERNAL
      GIT_REPOSITORY https://luajit.org/git/luajit-2.0.git
      GIT_TAG v2.1.0-beta3
      CONFIGURE_COMMAND ""
      BUILD_IN_SOURCE true
      BUILD_COMMAND cd src && msvcbuild.bat static
    )

    ExternalProject_Get_property(LUAJIT_EXTERNAL SOURCE_DIR)
    set(LUAJIT_INCLUDE ${SOURCE_DIR}/src)
    set(LUAJIT_LIB_DIR ${SOURCE_DIR}/src)
    set(LUAJIT_LIB ${LUAJIT_LIB_DIR}/lua51.lib)
  endif()

  # do this so cmake doesn't flip out...
  file(MAKE_DIRECTORY ${LUAJIT_INCLUDE})
  file(MAKE_DIRECTORY ${LUAJIT_LIB_DIR})
  file(TOUCH ${LUAJIT_LIB})

  add_library(luajit INTERFACE)
  add_dependencies(luajit LUAJIT_EXTERNAL)
  target_include_directories(luajit INTERFACE ${LUAJIT_INCLUDE})
  target_link_libraries(luajit INTERFACE ${LUAJIT_LIB})

  if(BB_LINUX)
    target_link_libraries(luajit INTERFACE dl)
  endif()
endif()
