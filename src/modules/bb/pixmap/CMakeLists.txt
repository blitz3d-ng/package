set(SOURCE pixmap.cpp pixmap.h)

IF(NOT BB_MOBILE)
  set(SOURCE ${SOURCE} freeimage.loader.cpp)
ENDIF()

add_library (bb.pixmap STATIC ${SOURCE})
target_link_libraries (bb.pixmap bb.blitz)

IF(NOT BB_MOBILE)
  target_link_libraries (bb.pixmap freeimage)
ENDIF()
