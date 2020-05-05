add_library(bbruntime_dll STATIC bbruntime_dll.cpp bbruntime_dll.h bbruntime_dll.rc dpi.manifest resource.h)
target_include_directories (bbruntime_dll PRIVATE ../../modules)

target_link_libraries(bbruntime_dll bb.stub)
