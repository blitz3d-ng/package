cmake_minimum_required (VERSION 3.24)
project (Blitz3D)

cmake_policy(SET CMP0069 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

cmake_policy(SET CMP0072 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0072 NEW)

cmake_policy(SET CMP0135 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(src/bin2h.cmake)

set(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH OFF)

if(ANDROID)
  find_package(Java 1.8 REQUIRED COMPONENTS Development)
  include(UseJava)
  set(CMAKE_JAVA_COMPILE_FLAGS -source 1.8 -target 1.8)
  set(ANDROID_JAR $ENV{ANDROID_HOME}/platforms/android-${ANDROID_PLATFORM}/android.jar)
endif()

# c++17/windows.h conflict
if (MSVC)
  add_compile_definitions(_HAS_STD_BYTE=0)
endif()

# TODO: make this toggleable
if(MSVC)
  set(BB_STATIC true)
  set(BB_SHARED false)
else()
  set(BB_STATIC false)
  set(BB_SHARED true)
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(BB_CLANG true)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(BB_GNU true)
endif()

# TODO: this will be extended a bit...
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES x86_64 OR CMAKE_HOST_SYSTEM_PROCESSOR MATCHES AMD64)
  set(BB_HOST_ARCH "x86_64")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL arm64)
  set(BB_HOST_ARCH "arm64")
endif()

if($ENV{ARCH})
  set(BB_ARCH $ENV{ARCH})
endif()

if(ARCH)
  set(BB_ARCH ${ARCH})
else()
  if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES x86_64 OR CMAKE_HOST_SYSTEM_PROCESSOR MATCHES AMD64)
    set(BB_ARCH "x86_64")
  elseif(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL arm64)
    set(BB_ARCH "arm64")
  endif()
endif()

# set symbol visibility to hidden to make dead code elimination more effective
if (NOT MSVC)
  add_compile_options(-fvisibility=hidden -fdata-sections -ffunction-sections)
endif()

if(NOT BB_HOST_ARCH STREQUAL BB_ARCH AND NOT BB_PLATFORM MATCHES mingw AND NOT BB_PLATFORM STREQUAL android AND NOT BB_PLATFORM STREQUAL win32 AND NOT BB_PLATFORM STREQUAL emscripten)
  if(BB_CLANG)
    add_compile_options(-arch ${BB_ARCH})
    add_link_options(-arch ${BB_ARCH})
  endif()

  set(BB_CROSSCOMPILE true)
endif()

IF(BB_ARCH MATCHES arm64 OR BB_ARCH MATCHES x86_64)
  set(BB64 true)
ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(BB64 true)
ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(BB32 true)
ENDIF()

IF(NOT BB_PLATFORM)
  IF(WIN32)
    IF(BB64)
      set (BB_PLATFORM win64)
    ELSEIF()
      set (BB_PLATFORM win32)
    ENDIF()
  ELSEIF(APPLE)
    set (BB_PLATFORM macos)
  ELSEIF(UNIX)
    set (BB_PLATFORM linux)
  ENDIF()
ENDIF()

IF(BB_PLATFORM MATCHES win32)
  set (BB_WIN32 true)
  add_definitions(-DBB_WIN32)
ELSEIF(BB_PLATFORM MATCHES win64)
  set (BB_WIN64 true)
  add_definitions(-DBB_WIN64)
ELSEIF(BB_PLATFORM MATCHES woa64)
  set (BB_WOA64 true)
  add_definitions(-DBB_WOA64)
ELSEIF(BB_PLATFORM MATCHES mingw32)
  set (BB_MINGW32 true)
  add_definitions(-DBB_MINGW32)
ELSEIF(BB_PLATFORM MATCHES mingw64)
  set (BB_MINGW64 true)
  add_definitions(-DBB_MINGW64)
ELSEIF(BB_PLATFORM MATCHES macos)
  set (BB_MACOS true)
  set (BB_MACH true)
  add_definitions(-DBB_MACOS -DBB_MACH)
ELSEIF(BB_PLATFORM MATCHES linux)
  set (BB_LINUX true)
  add_definitions(-DBB_LINUX)
ELSEIF(BB_PLATFORM MATCHES ios)
  set (BB_IOS true)
  set (BB_MACH true)
  add_definitions(-DBB_IOS -DBB_MACH)
ELSEIF(BB_PLATFORM MATCHES android)
  set (BB_ANDROID true)
  add_definitions(-DBB_ANDROID)
ELSEIF(BB_PLATFORM MATCHES ovr)
  set (BB_OVR true)
  set (OVR_HOME ${PROJECT_SOURCE_DIR}/ovr-sdk)
  add_definitions(-DBB_OVR)
ELSEIF(BB_PLATFORM MATCHES emscripten)
  set (BB_EMSCRIPTEN true)
  add_definitions(-DBB_EMSCRIPTEN)
ELSEIF(BB_PLATFORM MATCHES nx)
  set (BB_NX true)
  add_definitions(-DBB_NX)
ELSE()
  message(FATAL_ERROR "Unrecognized platform: ${BB_PLATFORM}")
ENDIF()

IF(BB_WIN32 OR BB_WIN64 OR BB_WOA64)
  set(BB_MSVC true)
  add_definitions(-DBB_MSVC)
ENDIF()

IF(BB_MINGW32 OR BB_MINGW64)
  set (BB_MINGW true)
  add_definitions(-DBB_MINGW)
ENDIF()

IF(BB_WIN32 OR BB_WIN64 OR BB_WOA64 OR BB_MINGW)
  set (BB_WINDOWS true)
  add_definitions(-DBB_WINDOWS)
ENDIF()

IF(BB_WINDOWS OR BB_MACOS OR BB_LINUX)
  set (BB_DESKTOP true)
  add_definitions(-DBB_DESKTOP)
ENDIF()

IF(BB_MACOS OR BB_LINUX)
  set (BB_POSIX true)
  add_definitions(-DBB_POSIX)
ENDIF()

IF(BB_IOS OR BB_ANDROID)
  set (BB_MOBILE true)
  add_definitions(-DBB_MOBILE)
ENDIF()

IF(BB_ANDROID OR BB_OVR)
  set (BB_NDK true)
  add_definitions(-DBB_NDK)
ENDIF()

IF(BB_EMSCRIPTEN)
  set (BB_WEB true)
  add_definitions(-DBB_WEB)
  set(CMAKE_SYSTEM_PROCESSOR wasm) # defaults to x86? not sure that makes sense.
ENDIF()

IF(NOT BB_ENV)
  set(BB_ENV debug)
ENDIF()

IF(BB_ENV MATCHES debug OR BB_ENV MATCHES test)
  set (CMAKE_BUILD_TYPE Debug)
  set (BB_DEBUG true)
  add_definitions(-DBB_DEBUG)

  # TODO: work to remove the need for this...
  add_definitions(-D_DEBUG)
ELSE()
  set(CMAKE_BUILD_TYPE MinSizeRel)
  # TODO: perhaps enable this as a build option for those interested in speed
  # over disk space.
  # set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  set(BB_RELEASE true)
  if(NOT BB_MINGW)
    set(ENABLE_LTO true)
  endif()
ENDIF()

if(BB_ENV MATCHES test)
  set(BB_COVERAGE "-fprofile-arcs -ftest-coverage")
endif()

IF(BB_EMSCRIPTEN)
  set (CMAKE_POSITION_INDEPENDENT_CODE OFF)
else()
  set (CMAKE_POSITION_INDEPENDENT_CODE ON)
ENDIF()

IF(BB_MSVC)
  if(BB32)
    set(BB_ARCH i686)
  endif()
  set(BB_VENDOR windows)
  set(BB_SYS win32)
ELSEIF(BB_MINGW)
  set(BB_VENDOR windows)
  set(BB_SYS mingw)
ELSEIF(BB_MACOS)
  execute_process(COMMAND sw_vers -productVersion OUTPUT_VARIABLE BB_PLATFORM_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(BB_VENDOR apple)
  set(BB_SYS darwin${BB_PLATFORM_VERSION})
ELSEIF(BB_LINUX)
  set(BB_VENDOR linux)
  set(BB_SYS gnu)
ELSEIF(BB_IOS)
  set(BB_PLATFORM_VERSION ${DEPLOYMENT_TARGET})
  set(BB_VENDOR apple)
  set(BB_SYS ${BB_PLATFORM}${DEPLOYMENT_TARGET})
ELSEIF(BB_ANDROID)
  set(BB_PLATFORM_VERSION ${ANDROID_PLATFORM})
  set(BB_VENDOR android)
  set(BB_SYS ${ANDROID_PLATFORM})
ELSEIF(BB_OVR)
  set(BB_PLATFORM_VERSION ${ANDROID_PLATFORM})
  set(BB_VENDOR ovr)
  set(BB_SYS ${ANDROID_PLATFORM})
ELSEIF(BB_EMSCRIPTEN)
  set(BB_ARCH wasm32)
  set(BB_VENDOR unknown)
  set(BB_SYS emscripten)
ELSEIF(BB_NX)
  set(BB_PLATFORM_VERSION ${CMAKE_CXX_COMPILER_VERSION})
  set(BB_VENDOR nx)
  set(BB_SYS devkitpro)
ENDIF()

IF(NOT BB_ARCH OR NOT BB_VENDOR OR NOT BB_SYS)
  message(FATAL_ERROR "Cannot generate target triple. Incomplete: ${BB_ARCH}-${BB_VENDOR}-${BB_SYS}")
ENDIF()

set(BB_TRIPLE "${BB_ARCH}-${BB_VENDOR}-${BB_SYS}")

if(OUTPUT_PATH)
  file(REAL_PATH ${OUTPUT_PATH} RELEASE_PATH)
else()
  set(RELEASE_PATH ${CMAKE_SOURCE_DIR}/_release)
endif()
set(TOOLCHAIN_PATH ${RELEASE_PATH}/bin/${BB_TRIPLE})
if(NOT BB_WINDOWS)
  set(TOOLCHAIN_BIN_PATH ${TOOLCHAIN_PATH})
else()
  set(TOOLCHAIN_BIN_PATH ${TOOLCHAIN_PATH}/..)
endif()

function(target_output_directory target dir)
  set_target_properties(${target}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${dir}
    ARCHIVE_OUTPUT_DIRECTORY ${dir}
    LIBRARY_OUTPUT_DIRECTORY ${dir}
  )

  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set_target_properties(${target}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${dir}
      ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${dir}
      LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${dir}
    )
  endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
endfunction()

function(target_output_to_toolchain target)
  set(dir ${TOOLCHAIN_PATH})
  get_target_property(target_type ${target} TYPE)
  if(target_type STREQUAL "STATIC_LIBRARY")
    if(BB_WINDOWS AND BB32)
      return()
    endif()
    set(dir ${TOOLCHAIN_PATH}/lib)
  elseif(BB_WINDOWS AND BB32)
    set(dir ${RELEASE_PATH}/bin)
  endif()

  target_output_directory(${target} ${dir})
endfunction()

function(target_output_to_bin target)
  target_output_directory(${target} ${RELEASE_PATH}/bin)
endfunction()

IF(BB_MSVC)
  IF(BB_STATIC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
    IF(NOT BB_DEBUG)
      add_compile_options(/MT)
    ELSE()
      add_compile_options(/MTd)
    ENDIF()
  ENDIF()

  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

IF(BB_IOS)
  set(ZLIB z)
ELSE()
  set(ZLIB zlibstatic)
ENDIF()

IF(BB_EMSCRIPTEN)
  add_link_options(-sUSE_PTHREADS)
  add_compile_options(-pthread)
ENDIF()

IF(BB_DEBUG AND BB_CLANG AND NOT BB_NDK)
  add_compile_definitions(BB_ASAN)
  add_compile_options(-fno-omit-frame-pointer -fsanitize=address)
  add_link_options(-fno-omit-frame-pointer -fsanitize=address)
ENDIF()

IF (BB_GNU)
  add_compile_options(-fdiagnostics-color=always)
ELSEIF (BB_CLANG)
  add_compile_options(-fcolor-diagnostics)
ENDIF ()

set(CMAKE_UNITY_BUILD true)

function(file_write_changes path src)
  set(changed true)
  if(EXISTS ${path})
    file(READ ${path} prev_src)
    if("${src}" STREQUAL "${prev_src}")
      set(changed false)
    endif()
  endif()

  if(changed)
    file(WRITE ${path} "${src}")
  endif()
endfunction()

if(APPLE)
  enable_language(OBJC) # TODO: needed after adding SDL...seems like a deep root cmake bug
endif()
add_subdirectory(deps EXCLUDE_FROM_ALL)

#if(NOT MSVC)
#  add_compile_options(-Wall -Werror
#    -Wno-error=deprecated-declarations
#  )
#endif()

add_definitions(-DBB_PLATFORM="${BB_PLATFORM}")
add_definitions(-DBB_ENV="${BB_ENV}")
add_definitions(-DBB_ARCH="${BB_ARCH}")
add_definitions(-DBB_TRIPLE="${BB_TRIPLE}")

IF(BB64)
  add_compile_definitions(BB64)
ELSE()
  add_compile_definitions(BB32)
ENDIF()

add_definitions(-DBB_SOURCE_ROOT="${CMAKE_SOURCE_DIR}")

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# legacy flags. TODO: remove them
add_definitions(-DPRO) # Blitz3D vs. Blitz2D
IF(BB_WINDOWS)
  add_definitions(-DDIRECTSOUND_VERSION=0x700) # this one may not be needed
ENDIF()

add_definitions(-DBB_COMMIT_HASH="${GIT_COMMIT_HASH}")
add_definitions(-DBB_BRANCH="${GIT_BRANCH}")

add_subdirectory(src/stdutil)
add_subdirectory(src/modules)
add_subdirectory(src/runtime)
add_subdirectory(src/tools)

# extras...
add_subdirectory(src/bindings)

file(WRITE ${TOOLCHAIN_PATH}/toolchain.toml "id = \"${BB_TRIPLE}\"
platform = \"${BB_PLATFORM}\"
platform_version = \"${BB_PLATFORM_VERSION}\"
arch = \"${BB_ARCH}\"
\n${RUNTIMES_IFACE}
${MODULES_IFACE}
")
