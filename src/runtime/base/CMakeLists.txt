set(SOURCES bbruntime_dll.h runtime.cpp)
if(BB_WINDOWS)
  set(SOURCES ${SOURCES} bbruntime_dll.cpp bbruntime_dll.rc resource.h dpi.manifest)
endif()

add_library(bbruntime_dll OBJECT ${SOURCES})
target_include_directories (bbruntime_dll PRIVATE ../../modules)

target_link_libraries(bbruntime_dll bb.stub)
