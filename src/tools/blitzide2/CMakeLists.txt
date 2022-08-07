if (BB_MSVC)
  set(CMAKE_UNITY_BUILD false)
endif()

if (TARGET wxstc AND TARGET wxwebview)
  add_executable(ide2 blitzide2.rc dpi.h dpi.cpp
    MainFrame.h MainFrame.cpp
    HtmlHelp.h HtmlHelp.cpp
    FileView.h FileView.cpp
    PreferencesDialog.h PreferencesDialog.cpp
    BlitzCC.h BlitzCC.cpp

    BuildDialog.h BuildDialog.cpp
    main.cpp
  )
  target_output_to_bin(ide2)
  target_link_libraries(ide2 wxwebview wxstc toml11)

  if(BB_MSVC)
    # TODO: find out why wxUSE_DPI_AWARE_MANIFEST isn't working...
    target_sources(ide2 PUBLIC ../../../deps/wxwidgets/tree/include/wx/msw/wx_dpi_aware_pmv2.manifest)
    target_link_options(ide2 PUBLIC /ENTRY:\"WinMainCRTStartup\" /subsystem:windows)
  endif()
endif()
