if (BB_MSVC)
  set(CMAKE_UNITY_BUILD false)
endif()

if (TARGET wxstc AND TARGET wxwebview)
  add_executable(ide2 blitzide2.rc dpi.h dpi.cpp
    MainFrame.h MainFrame.cpp
    HtmlHelp.h HtmlHelp.cpp
    FileView.h FileView.cpp
    BlitzCC.h BlitzCC.cpp
    main.cpp
  )
  target_output_to_bin(ide2)
  target_link_libraries(ide2 wxwebview wxstc)

  if(BB_MSVC)
    target_link_options(ide2 PUBLIC /ENTRY:\"WinMainCRTStartup\" /subsystem:windows)
  endif()
endif()
