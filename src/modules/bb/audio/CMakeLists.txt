set (SOURCES channel.cpp channel.h sound.cpp sound.h driver.cpp driver.h audio.cpp audio.h stream.cpp stream.h)

IF(BB_WINDOWS OR BB_MACOS OR BB_LINUX)
 set (SOURCES ${SOURCES} ogg_stream.cpp ogg_stream.h wav_stream.cpp wav_stream.h)
ENDIF()

add_library(bb.audio STATIC ${SOURCES})
target_link_libraries (bb.audio bb.blitz bb.runtime vorbis ogg)
