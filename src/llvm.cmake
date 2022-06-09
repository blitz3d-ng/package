##
# Run `make llvm` to build a local copy of LLVM and activate this.

set(llvm_path ${PROJECT_SOURCE_DIR}/_release/toolchains/llvm)

if (EXISTS ${llvm_path})
  set(LLVM_ROOT ${llvm_path})

  # set(CMAKE_C_COMPILER ${LLVM_ROOT}/bin/clang)
  # set(CMAKE_CXX_COMPILER ${LLVM_ROOT}/bin/clang++)
  # set(CMAKE_OBJC_COMPILER ${LLVM_ROOT}/bin/clang++)

  # set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  # set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
  # set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
  # set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif()
