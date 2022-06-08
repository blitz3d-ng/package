# this is a hack/patch fix a "multiple definitions" linking error under mingw

add_library(glfw.guid STATIC guid.cpp)
