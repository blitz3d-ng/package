#include "system.windows.h"
#include "driver.h"
#include <bb/blitz/module.h>
#include "../../stdutil/stdutil.h"

extern inline void debugBank( bbBank *b );
extern inline void debugBank( bbBank *b,int offset );

int BBCALL bbCallDLL( BBStr *dll,BBStr *fun,bbBank *in,bbBank *out ){
	if( bb_env.debug ){
		if( in ) debugBank( in );
		if( out ) debugBank( out );
	}
	int t=((WindowsSystemDriver*)sys_driver)->callDll( *dll,*fun,
		in ? in->data : 0,in ? in->size : 0,
		out ? out->data : 0,out ? out->size : 0 );
	delete dll;delete fun;
	return t;
}

BBMODULE_CREATE( system_windows ){
	if( !sys_driver ){
		sys_driver=d_new WindowsSystemDriver();
	}
	return true;
}

BBMODULE_LINK( system_windows ){
	rtSym( "%CallDLL$dll_name$func_name%in_bank=0%out_bank=0",bbCallDLL );
}

BBMODULE_DESTROY( system_windows ){
	return true;
}
