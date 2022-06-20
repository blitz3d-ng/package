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

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(BB64 true)
ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(BB32 true)
ENDIF()

if(BB64 AND NOT MSVC AND NOT MINGW)
  if (DEFINED ENV{LLVM_ROOT})
    set(llvm_path $ENV{LLVM_ROOT})
  else()
    set(llvm_path ${PROJECT_SOURCE_DIR}/_release/toolchains/llvm)
  endif()

  if (EXISTS ${llvm_path})
    set(LLVM_ROOT ${llvm_path})
    find_package(LLVM 14.0.4 REQUIRED CONFIG PATHS ${LLVM_ROOT})
    find_package(LLD REQUIRED CONFIG PATHS ${LLVM_ROOT})

    # if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #   add_link_options(-B${llvm_path}/bin -fuse-ld=lld)
    # endif()

    # if(NOT MSVC)
    #   set(CMAKE_C_COMPILER /usr/bin/clang)
    #   set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    #   set(CMAKE_OBJC_COMPILER /usr/bin/clang++)
    # endif()
  endif()
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
ELSEIF(BB_PLATFORM MATCHES mingw64)
  set (BB_MINGW64 true)
  add_definitions(-DBB_MINGW64)
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

IF(BB_MINGW32 OR BB_MINGW64)
  set (BB_MINGW true)
  add_definitions(-DBB_MINGW)
ENDIF()

IF(BB_WIN32 OR BB_WIN64 OR BB_MINGW)
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

IF(BB_EMSCRIPTEN)
  set (BB_WEB true)
  add_definitions(-DBB_WEB)
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
  set(CMAKE_BUILD_TYPE Release)
  if(NOT BB_MINGW)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
  set(BB_RELEASE true)
ENDIF()

if(BB_ENV MATCHES test)
  set(BB_COVERAGE "-fprofile-arcs -ftest-coverage")
endif()

IF(NOT BB_EMSCRIPTEN)
  set (CMAKE_POSITION_INDEPENDENT_CODE ON)
ENDIF()

set (RELEASE_PATH ${CMAKE_SOURCE_DIR}/_release)
set (BUILD_PATH   ${RELEASE_PATH}/toolchains/${BB_PLATFORM})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_PATH}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_PATH}/lib/${BB_ENV})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_PATH}/lib/${BB_ENV})

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
  string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BUILD_PATH}/bin )
  set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BUILD_PATH}/lib/${BB_ENV} )
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BUILD_PATH}/lib/${BB_ENV}} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

function(target_output_to_bin target)
  set_target_properties(${target}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${RELEASE_PATH}/bin
    LIBRARY_OUTPUT_DIRECTORY ${RELEASE_PATH}/bin
  )

  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set_target_properties(${target}
      PROPERTIES
      CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RELEASE_PATH}/bin
      CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RELEASE_PATH}/bin
    )
  endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
endfunction()

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

IF(BB_DEBUG AND "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  add_compile_definitions(BB_ASAN)
  add_compile_options(-fno-omit-frame-pointer -fsanitize=address)
  add_link_options(-fno-omit-frame-pointer -fsanitize=address)
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

IF (LLVM_ROOT)
  add_compile_definitions(BB_LLVM)
ENDIF()

add_definitions(-DBB_COMMIT_HASH="${GIT_COMMIT_HASH}")
add_definitions(-DBB_BRANCH="${GIT_BRANCH}")

add_subdirectory(stdutil)
add_subdirectory(src/blitzide2)

IF(BB_MSVC)
  add_subdirectory(src/legacy/blitzide)
  add_subdirectory(src/legacy/debugger)
ENDIF()

IF(BB_DESKTOP OR BB_EMSCRIPTEN)
  add_subdirectory(src/toolchain/compiler)
ENDIF()

IF(BB_WINDOWS)
  add_subdirectory(src/legacy/bblaunch)
  add_subdirectory(src/legacy/linker)
ENDIF()

add_subdirectory(src/modules)
add_subdirectory(src/runtime)
add_subdirectory(src/bindings)

# depends on bb.stub, so we add it in here.
IF(BB_WINDOWS)
  add_subdirectory(src/legacy/bbruntime_dll)
ENDIF()
