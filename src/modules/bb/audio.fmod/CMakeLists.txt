IF(BB_WINDOWS)
  bb_start_module(audio.fmod)
  set(DEPENDS_ON bb.audio bb.runtime)
  set(SOURCES audio.fmod.h sound.cpp sound.h driver.cpp driver.h)
  set(LIBS fmodvc)

  bb_end_module()
  target_include_directories(bb.audio.fmod PRIVATE ../../../../common/include)
ENDIF()
