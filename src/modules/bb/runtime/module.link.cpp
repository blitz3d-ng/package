// AUTOGENERATED. DO NOT EDIT.
// RUN `make` TO UPDATE.

#include <bb/blitz/module.h>
#include <bb/runtime/runtime.h>

BBMODULE_LINK( runtime ){
	rtSym( "ShowPointer","bbShowPointer",bbShowPointer );
	rtSym( "HidePointer","bbHidePointer",bbHidePointer );
	rtSym( "RuntimeError$message","bbRuntimeError",bbRuntimeError );
	rtSym( "MoveMouse%x%y","bbMoveMouse",bbMoveMouse );
}
