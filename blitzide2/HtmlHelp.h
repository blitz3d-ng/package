#ifndef HTMLHELP_H
#define HTMLHELP_H

#include "std.h"
#include <wx/webview.h>

#if !wxUSE_WEBVIEW_WEBKIT && !wxUSE_WEBVIEW_IE
#error "You need wxWebView to build this."
#endif

class HtmlHelp : public wxPanel{
private:
  wxWebView *browser;
public:
  HtmlHelp( wxWindow *parent,wxWindowID id );

  void OnNavigating( wxWebViewEvent& event );

  void GoHome();
  bool GoBack();
  bool GoForward();

  bool CanGoBack();
  bool CanGoForward();
};

wxDECLARE_EVENT(OPEN_EVENT, wxCommandEvent);


#endif
