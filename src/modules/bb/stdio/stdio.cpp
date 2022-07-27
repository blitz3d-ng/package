
#include "stdio.h"

#include <iostream>
using namespace std;

#ifdef BB_IOS
#include <syslog.h>
#endif

void BBCALL bbWriteStdout( BBStr *str ){
#ifdef BB_IOS
	syslog(LOG_WARNING, "%s", str->c_str());
#endif
  cout<<*str;delete str;
}

void BBCALL bbWriteStderr( BBStr *str ){
#ifdef BB_IOS
	syslog(LOG_WARNING, "Blitz3D: %s\n", str->c_str());
#endif
  cerr<<*str;delete str;
}

BBMODULE_CREATE( stdio ){
  return true;
}

BBMODULE_DESTROY( stdio ){
  return true;
}
