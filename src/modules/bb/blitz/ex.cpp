#include "module.h"
#include "ex.h"
#include <cstring>

bbEx::bbEx( const char *e ){
	err=e?std::string( e ):"";
}

bbEx::~bbEx(){
}
