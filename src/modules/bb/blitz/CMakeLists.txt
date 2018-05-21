add_library (bb.blitz STATIC app.cpp app.h basic.cpp basic.h debug.cpp debug.h env.cpp env.h ex.cpp ex.h module.h commands.h)
target_link_libraries(bb.blitz stdutil)
