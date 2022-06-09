cmake_minimum_required(VERSION 3.21)
project(llvm-build)

set (CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../_release/toolchains/llvm)

# customize for our needs.
set(LLVM_ENABLE_PROJECTS  "lld" CACHE STRING "" FORCE)
set(LLVM_ENABLE_RUNTIMES  "" CACHE STRING "" FORCE)
set(LLVM_TARGETS_TO_BUILD "AArch64;ARM;WebAssembly;X86" CACHE STRING "" FORCE)

add_subdirectory(tree/llvm)
