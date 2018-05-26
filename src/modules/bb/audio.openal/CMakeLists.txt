bb_start_module(audio.openal)
set(DEPENDS_ON bb.audio)
set(SOURCES audio.openal.h audio.openal.cpp commands.h)
set(LIBS )

IF(BB_MACOS)
  set(LIBS "-framework OpenAL")
ENDIF()

bb_end_module()
