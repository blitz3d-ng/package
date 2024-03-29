add_compile_options(-w) # suppress warnings

add_library(crossguid STATIC src/guid.h src/guid.cpp)
target_output_to_toolchain(crossguid)
target_include_directories(crossguid SYSTEM PUBLIC src)

IF(NOT BB_MSVC)
  target_compile_options(crossguid PRIVATE -Wno-c++11-narrowing)
ENDIF()

IF(BB_WINDOWS)
  target_compile_definitions(crossguid PRIVATE -DGUID_WINDOWS)
ELSEIF(BB_MACOS OR BB_IOS)
  target_compile_definitions(crossguid PRIVATE -DGUID_CFUUID)
  target_link_libraries(crossguid "-framework CoreFoundation")
ELSEIF(BB_LINUX)
  target_link_libraries(crossguid uuid)
  target_compile_definitions(crossguid PRIVATE -DGUID_LIBUUID)
ELSEIF(BB_ANDROID OR BB_OVR)
  target_compile_definitions(crossguid PRIVATE -DGUID_ANDROID)
ENDIF()
