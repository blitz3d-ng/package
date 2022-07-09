IF(BB_IOS)
  set(CMAKE_UNITY_BUILD false)
ENDIF()

bb_start_module(pixmap)
set(DEPENDS_ON bb.blitz)
set(SOURCES pixmap.cpp pixmap.h)

IF(BB_DESKTOP)
  set(SOURCES ${SOURCES} freeimage.loader.cpp)
  set(LIBS freeimage ${ZLIB})
ELSEIF(BB_IOS)
  set(SOURCES ${SOURCES} uikit.loader.m)
  set(LIBS "-framework UIKit")
ENDIF()

bb_end_module()
