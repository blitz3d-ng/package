##
# Run `make llvm` to build a local copy of LLVM and activate this or use
# one of the prebuilt docker containers.

if (DEFINED ENV{LLVM_ROOT})
  set(llvm_path $ENV{LLVM_ROOT})
else()
  set(llvm_path ${PROJECT_SOURCE_DIR}/_release/toolchains/llvm)
endif()

if (EXISTS ${llvm_path})
  set(LLVM_ROOT ${llvm_path})

  add_link_options(-B${llvm_path}/bin -fuse-ld=lld)
  set(CMAKE_C_COMPILER /usr/bin/clang)
  set(CMAKE_CXX_COMPILER /usr/bin/clang++)
  set(CMAKE_OBJC_COMPILER /usr/bin/clang++)
endif()
