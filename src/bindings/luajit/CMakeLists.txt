set(LUAJIT_TREE ${PROJECT_SOURCE_DIR}/deps/luajit/tree)

macro(blitzlua runtime)
  add_executable(blitzlua.${runtime} ../main.cpp ../runtime/${runtime}.c)
  target_link_libraries(blitzlua.${runtime} runtime.${runtime}.static luajit)
  target_link_options(blitzlua.${runtime} PRIVATE ${LUAJIT_TREE}/src/libluajit.a)

  if(BB_MACOS)
    target_link_options(blitzlua.${runtime} PRIVATE -pagezero_size 10000 -image_base 100000000)
  endif()
endmacro()

add_subdirectory(runtime)
