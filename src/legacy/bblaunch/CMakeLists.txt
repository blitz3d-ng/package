set(CMAKE_MFC_FLAG 0)
add_executable(bblaunch WIN32 bblaunch.cpp checkdx.cpp checkdx.h checkie.cpp checkie.h bblaunch.rc resource.h)
set_target_properties(bblaunch PROPERTIES
  OUTPUT_NAME "Blitz3D"
  LINK_FLAGS "/ENTRY:\"wWinMainCRTStartup\" /subsystem:windows /p:CharacterSet=Unicode /w4100 /w4189"
)
target_compile_definitions(bblaunch PRIVATE UNICODE _UNICODE _WIN32_WINNT=_WIN32_WINNT_WINXP)
target_link_libraries(bblaunch dxguid kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32)
