
#include "stdio.h"
#include <bb/blitz/app.h>

void BBCALL bbWriteStdout( BBStr *str ){
	LOGI( "%s",str->c_str() );
	delete str;
}

void BBCALL bbWriteStderr( BBStr *str ){
	LOGE( "%s",str->c_str() );
	delete str;
}

BBMODULE_CREATE( stdio ){
  return true;
}

BBMODULE_DESTROY( stdio ){
  return true;
}
