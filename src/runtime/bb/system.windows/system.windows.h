#ifndef BB_SYSTEM_WINDOWS_H
#define BB_SYSTEM_WINDOWS_H

#include <bb/blitz/blitz.h>
#include <bb/bank/bank.h>

int BBCALL bbCallDLL( BBStr *dll,BBStr *fun,bbBank *in,bbBank *out );

#endif
