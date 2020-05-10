include(ExternalProject)
set(LUAJIT_ROOT ${CMAKE_CURRENT_BINARY_DIR}/root)

if(BB_DESKTOP)
  if(BB_MACOS OR BB_LINUX)
    if(BB_MACOS)
      set(MAKE_FLAGS MACOSX_DEPLOYMENT_TARGET=10.14)
    endif()

    ExternalProject_Add(deps-luajit
      GIT_REPOSITORY https://luajit.org/git/luajit-2.0.git
      GIT_TAG v2.1.0-beta3
      CONFIGURE_COMMAND sed -i'' "s|/usr/local|${LUAJIT_ROOT}|g" Makefile
      BUILD_IN_SOURCE true
      BUILD_COMMAND make ${MAKE_FLAGS}
    )

    set(LUAJIT_INCLUDE ${LUAJIT_ROOT}/include/luajit-2.1)
    set(LUAJIT_LIB ${LUAJIT_ROOT}/lib/libluajit-5.1.a)
  elseif(BB_WINDOWS)
    ExternalProject_Add(deps-luajit
      GIT_REPOSITORY https://luajit.org/git/luajit-2.0.git
      GIT_TAG v2.1.0-beta3
      CONFIGURE_COMMAND ""
      BUILD_IN_SOURCE true
      BUILD_COMMAND cd src && msvcbuild.bat static
    )

    set(LUAJIT_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/deps-luajit-prefix/src/deps-luajit/src)
    set(LUAJIT_LIB ${CMAKE_CURRENT_BINARY_DIR}/deps-luajit-prefix/src/deps-luajit/src/lua51.lib)
  endif()

  # do this so cmake doesn't flip out...
  file(MAKE_DIRECTORY ${LUAJIT_INCLUDE})

  add_library(luajit STATIC IMPORTED GLOBAL)
  add_dependencies(luajit deps-luajit)
  target_include_directories(luajit INTERFACE ${LUAJIT_INCLUDE})
  set_property(TARGET luajit PROPERTY IMPORTED_LOCATION ${LUAJIT_LIB})

  if(BB_LINUX)
    target_link_libraries(luajit INTERFACE dl)
  endif()
endif()
