bb_start_module(audio)
set(DEPENDS_ON bb.runtime)
set(SOURCES channel.cpp channel.h sound.cpp sound.h driver.cpp driver.h audio.cpp audio.h stream.cpp stream.h)

IF(TARGET ogg AND TARGET vorbis)
 set (SOURCES ${SOURCES} ogg_stream.cpp ogg_stream.h wav_stream.cpp wav_stream.h)
 set (LIBS ${LIBS} vorbisfile vorbis ogg)
ENDIF()

IF(TARGET minimp3)
 set (SOURCES ${SOURCES} mp3_stream.cpp mp3_stream.h)
 set (INTERFACE_LIBS ${INTERFACE_LIBS} minimp3)
ENDIF()

bb_end_module()
