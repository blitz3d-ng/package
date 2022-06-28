set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)


IF(BB_DESKTOP AND NOT BB_CROSSCOMPILE)
  add_subdirectory(compiler)
  add_subdirectory(blitzide2)
  add_subdirectory(bblaunch2)

  IF(BB_WINDOWS)
    add_subdirectory(bblaunch)
  ENDIF()
ENDIF()

IF(BB_MSVC AND NOT BB_CROSSCOMPILE)
  add_subdirectory(src/legacy/blitzide)
  add_subdirectory(src/legacy/debugger)
ENDIF()
