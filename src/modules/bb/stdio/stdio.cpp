
#include "stdio.h"

#include <iostream>
using namespace std;

void BBCALL bbWriteStdout( BBStr *str ){
  cout<<*str;delete str;
}

void BBCALL bbWriteStderr( BBStr *str ){
  cerr<<*str;delete str;
}

BBMODULE_CREATE( stdio ){
  return true;
}

BBMODULE_DESTROY( stdio ){
  return true;
}
