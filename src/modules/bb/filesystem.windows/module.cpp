
#include "../stdutil/stdutil.h"
#include <bb/blitz/module.h>
#include "driver.h"

BBMODULE_CREATE( filesystem_windows ){
  if( !gx_filesys ){
    gx_filesys=d_new WindowsFileSystem();
  }
  return true;
}

BBMODULE_DESTROY( filesystem_windows ){
  return true;
}

BBMODULE_LINK( filesystem_windows ){
}
