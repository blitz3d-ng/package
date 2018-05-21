add_library (bb.audio.openal STATIC audio.openal.h audio.openal.cpp commands.h)
target_link_libraries (bb.audio.openal bb.blitz bb.runtime bb.audio)

IF(BB_MACOS)
  target_link_libraries (bb.audio.openal "-framework OpenAL")
ENDIF()
