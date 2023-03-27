bb_start_module(pixmap)
set(DEPENDS_ON bb.blitz)
set(SOURCES pixmap.cpp pixmap.h)

IF(TARGET freeimage)
  set(SOURCES ${SOURCES} freeimage.loader.cpp)
  set(LIBS freeimage ${ZLIB})
ELSEIF(BB_IOS)
  set(SOURCES ${SOURCES} uikit.loader.m)
  set(SYSTEM_LIBS "-framework UIKit" "-framework Foundation")
ENDIF()

bb_end_module()
