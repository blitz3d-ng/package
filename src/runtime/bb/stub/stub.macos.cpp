
#include <bb/stub/stub.h>

#include <iostream>
#include <cstdlib>
using namespace std;

void sue( const char *t ){
  cerr<<t<<endl;
  exit(1);
}

void bbMain();

int main(){
  if( !bbruntime_create() ) return 1;
  bbMain();
  bbruntime_destroy();
  return 0;
}
