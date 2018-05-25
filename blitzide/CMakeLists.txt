IF(BB_WINDOWS AND NOT BB_MINGW32)
  set(CMAKE_MFC_FLAG 1)
  add_executable(ide about.cpp about.h blitzide.cpp blitzide.h editor.cpp editor.h funclist.cpp funclist.h htmlhelp.cpp htmlhelp.h libs.cpp libs.h mainframe.cpp mainframe.h prefs.cpp prefs.h stdafx.cpp stdafx.h tabber.cpp tabber.h dpi.cpp dpi.h blitzide.rc)
  set_target_properties(ide PROPERTIES LINK_FLAGS "/ENTRY:\"WinMainCRTStartup\"")
  target_compile_definitions(ide PRIVATE -D_WIN32_WINNT=_WIN32_WINNT_WINXP -DNO_WARN_MBCS_MFC_DEPRECATION /w4100 /w4189)
  target_link_libraries(ide libcmtd)
ENDIF()

# targetdir ../_release/bin
#
# warnings Off
# disablewarnings {  }
#
# flags { WinMain MFC }
