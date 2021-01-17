bb_start_module(pixmap)
set(DEPENDS_ON bb.blitz)
set(SOURCES pixmap.cpp pixmap.h)

IF(BB_DESKTOP)
  set(SOURCES ${SOURCES} freeimage.loader.cpp)
  set(LIBS freeimage)

  # TODO: remove the need for this. here so bb-config includes it in builds
  set(LIBS ${LIBS} jpeg openexr openjpeg png raw_ tiff4 webp ${ZLIB})
ELSEIF(BB_IOS)
  set(SOURCES ${SOURCES} uikit.loader.m)
  set(LIBS "-framework UIKit")
ENDIF()

bb_end_module()
