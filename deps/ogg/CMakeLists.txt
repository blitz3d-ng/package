IF(NOT BB_WINDOWS)
  add_library(ogg STATIC src/src/bitwise.c src/src/framing.c)

  target_include_directories(ogg SYSTEM PUBLIC include src/include)
ENDIF()
