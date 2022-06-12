cmake_minimum_required (VERSION 3.16)
project (Blitz3D)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(src/bin2h.cmake)

# TODO: make this toggleable
set(BB_STATIC false)
set(BB_SHARED true)

# TODO: this will be extended a bit...
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES x86_64)
  set(BB_ARCH "x86")
endif()

IF(NOT BB_PLATFORM)
  IF(WIN32)
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set (BB_PLATFORM win64)
    ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
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
  set (BB_MSVC true)
  add_definitions(-DBB_WIN32 -DBB_MSVC)
ELSEIF(BB_PLATFORM MATCHES win64)
  set (BB_WIN64 true)
  set (BB_MSVC true)
  add_definitions(-DBB_WIN64 -DBB_MSVC)
ELSEIF(BB_PLATFORM MATCHES mingw32)
  set (BB_MINGW32 true)
  add_definitions(-DBB_MINGW32)
ELSEIF(BB_PLATFORM MATCHES macos)
  set (BB_MACOS true)
  add_definitions(-DBB_MACOS)
ELSEIF(BB_PLATFORM MATCHES linux)
  set (BB_LINUX true)
  add_definitions(-DBB_LINUX)
ELSEIF(BB_PLATFORM MATCHES ios)
  set (BB_IOS true)
  add_definitions(-DBB_IOS)
ELSEIF(BB_PLATFORM MATCHES android)
  set (BB_ANDROID true)
  add_definitions(-DBB_ANDROID)
ELSEIF(BB_PLATFORM MATCHES emscripten)
  set (BB_EMSCRIPTEN true)
  add_definitions(-DBB_EMSCRIPTEN)
ELSEIF(BB_PLATFORM MATCHES nx)
  set (BB_NX true)
  add_definitions(-DBB_NX)
ELSE()
  message(FATAL_ERROR "Unrecognized platform: ${BB_PLATFORM}")
ENDIF()

IF(BB_WIN32 OR BB_WIN64 OR BB_MINGW32)
  set (BB_WINDOWS true)
  add_definitions(-DBB_WINDOWS)
ENDIF()

IF(BB_WINDOWS OR BB_MACOS OR BB_LINUX)
  set (BB_DESKTOP true)
  add_definitions(-DBB_DESKTOP)
ENDIF()

IF(BB_IOS OR BB_ANDROID)
  set (BB_MOBILE true)
  add_definitions(-DBB_MOBILE)
ENDIF()

IF(BB_EMSCRIPTEN)
  set (BB_WEB true)
  add_definitions(-DBB_WEB)
ENDIF()

IF(BB_WIN32 OR BB_MINGW32)
  set(BB32 true)
ELSE()
  set(BB64 true)
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
  set (CMAKE_BUILD_TYPE Release)
  set (CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  set (BB_RELEASE true)
ENDIF()

if(BB_ENV MATCHES test)
  set(BB_COVERAGE "-fprofile-arcs -ftest-coverage")
endif()

IF(NOT BB_EMSCRIPTEN)
  set (CMAKE_POSITION_INDEPENDENT_CODE ON)
ENDIF()

set (BUILD_PATH ${CMAKE_SOURCE_DIR}/_release/toolchains/${BB_PLATFORM})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_PATH}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_PATH}/lib/${BB_ENV})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_PATH}/lib/${BB_ENV})

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BUILD_PATH}/bin )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BUILD_PATH}/lib/${BB_ENV} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BUILD_PATH}/lib/${BB_ENV}} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

IF(BB_MSVC)
  IF(BB_STATIC)
    include("./src/msvc.cmake")
    configure_msvc_runtime()
  ENDIF()

  IF(NOT BB_DEBUG)
    # add_link_options(/GL) # TODO: only apply to executables/dlls
    # add_compile_options(/LTCG)
  ENDIF()

  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

IF(NOT BB_IOS)
  set(ZLIB zlibstatic)
ELSE()
  set(ZLIB z)
ENDIF()

IF(BB_EMSCRIPTEN)
  add_link_options(-sUSE_PTHREADS)
  add_compile_options(-pthread)
ENDIF()

IF(BB_DEBUG AND USE_LLVM)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls)
  add_link_options(-fsanitize=address)
ENDIF()

IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  add_compile_options (-fcolor-diagnostics)
ENDIF ()

add_subdirectory(deps)

add_definitions(-DBB_PLATFORM="${BB_PLATFORM}")
add_definitions(-DBB_ENV="${BB_ENV}")
add_definitions(-DBB_ARCH="${BB_ARCH}")

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

IF (LLVM_ROOT)
  add_compile_definitions(BB_LLVM)
ENDIF()

add_definitions(-DBB_COMMIT_HASH="${GIT_COMMIT_HASH}")
add_definitions(-DBB_BRANCH="${GIT_BRANCH}")

add_subdirectory(stdutil)
add_subdirectory(src/blitzide2)

IF(BB_MSVC)
  add_subdirectory(src/legacy/blitzide)
  add_subdirectory(src/legacy/bblaunch)
  add_subdirectory(src/legacy/debugger)
ENDIF()

IF(BB_DESKTOP OR BB_EMSCRIPTEN)
  add_subdirectory(src/toolchain/compiler)
ENDIF()

IF(BB_WINDOWS)
  add_subdirectory(src/legacy/linker)
ENDIF()

add_subdirectory(src/modules)
add_subdirectory(src/runtime)
add_subdirectory(src/bindings)

# depends on bb.stub, so we add it in here.
IF(BB_WINDOWS)
  add_subdirectory(src/legacy/bbruntime_dll)
ENDIF()
