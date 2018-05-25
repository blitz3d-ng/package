add_library (bb.runtime.glfm STATIC runtime.glfm.cpp runtime.glfm.h main.cpp sample.c file_compat.h)
target_link_libraries (bb.runtime.glfm bb.runtime bb.system bb.input bb.graphics bb.blitz3d.gles2 glfm)
