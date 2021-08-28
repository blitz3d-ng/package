add_executable(ide2 blitzide2.rc dpi.h dpi.cpp HtmlHelp.h HtmlHelp.cpp FileView.h FileView.cpp main.cpp)
target_link_libraries(ide2 wxwebview wxstc)
