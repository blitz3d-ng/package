set(CMAKE_MFC_FLAG 1)

add_library(debugger SHARED debugger.rc debuggerapp.cpp debuggerapp.h debugtree.cpp debugtree.h mainframe.cpp mainframe.h prefs.cpp prefs.h sourcefile.cpp sourcefile.h stdafx.cpp stdafx.h tabber.cpp tabber.h dpi.cpp dpi.h ../../../stdutil/dpi.manifest debugger.h)

target_include_directories(debugger PRIVATE ../../../modules)
target_link_libraries(debugger stdutil nlohmann_json)
target_output_to_bin(debugger)

target_compile_definitions(debugger PRIVATE _WIN32_WINNT=_WIN32_WINNT_WINXP _WINDLL _USRDLL)
if(BB_SHARED)
  target_compile_definitions(debugger PRIVATE _AFXDLL)
endif()

target_link_options(debugger PRIVATE /subsystem:windows /NOIMPLIB /NOEXP)

if(BB_WIN64)
  set_target_properties(debugger PROPERTIES OUTPUT_NAME "debugger64")
endif()
