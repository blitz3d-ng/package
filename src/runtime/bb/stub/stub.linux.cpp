
#include <bb/stub/stub.h>

extern "C" void bbMain();

int main(){
  if( !bbruntime_create() ) return 1;
  bbMain();
  bbruntime_destroy();
  return 0;
}
