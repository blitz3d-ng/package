IF(BB_WINDOWS)
  add_library (bb.system.windows STATIC system.windows.cpp system.windows.h module.link.cpp)
  target_link_libraries (bb.system.windows bb.system bb.bank winmm kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32)
ENDIF()
