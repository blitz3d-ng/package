#ifndef BB_SYSTEM_WINDOWS_H
#define BB_SYSTEM_WINDOWS_H

#include "../blitz/module.h"
#include "../bbruntime/basic.h"
#include "../src/runtime/bank/bank.h"

int BBCALL bbCallDLL( BBStr *dll,BBStr *fun,bbBank *in,bbBank *out );

#endif
