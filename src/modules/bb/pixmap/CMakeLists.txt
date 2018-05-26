bb_start_module(pixmap)
set(DEPENDS_ON bb.blitz)
set(SOURCES pixmap.cpp pixmap.h)

IF(NOT BB_MOBILE)
  set(SOURCES ${SOURCES} freeimage.loader.cpp)
  set(LIBS freeimage)
ENDIF()

bb_end_module()
