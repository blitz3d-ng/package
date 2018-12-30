find_package(wxWidgets COMPONENTS std webview stc)

if(wxWidgets_FOUND)
  include(${wxWidgets_USE_FILE})
  add_executable(ide2 blitzide2.rc dpi.h dpi.cpp HtmlHelp.h HtmlHelp.cpp FileView.h FileView.cpp main.cpp)
  target_link_libraries(ide2 ${wxWidgets_LIBRARIES})
endif()
