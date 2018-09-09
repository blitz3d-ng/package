if(BB_MACOS)
  bb_start_module(audio.openal)
  set(DEPENDS_ON bb.audio)
  set(SOURCES audio.openal.h audio.openal.cpp commands.h)

  if(BB_MACOS)
    set(LIBS "-framework OpenAL")
  endif()

  bb_end_module()
endif()
