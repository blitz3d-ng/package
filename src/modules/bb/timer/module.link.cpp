// AUTOGENERATED. DO NOT EDIT.
// RUN `make` TO UPDATE.

#include <bb/blitz/module.h>
#include <bb/timer/timer.h>

BBMODULE_LINK( timer ){
	rtSym( "%CreateTimer%hertz",bbCreateTimer );
	rtSym( "%WaitTimer%timer",bbWaitTimer );
	rtSym( "FreeTimer%timer",bbFreeTimer );
}
