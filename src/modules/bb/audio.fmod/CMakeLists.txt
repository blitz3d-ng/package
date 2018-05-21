add_library (bb.audio.fmod STATIC audio.fmod.h sound.cpp sound.h driver.cpp driver.h)
target_link_libraries (bb.audio.fmod bb.blitz bb.runtime bb.audio)
