bb_start_module(blitz)
set(SOURCES app.cpp app.h basic.cpp basic.h debug.cpp debug.h env.cpp env.h ex.cpp ex.h module.h commands.h)
set(LIBS stdutil)

if(BB_NDK)
  set(LIBS ${LIBS} log)
endif()

bb_end_module()
