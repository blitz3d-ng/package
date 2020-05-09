#ifndef BB_SYSTEM_WINDOWS_COMMANDS_H
#define BB_SYSTEM_WINDOWS_COMMANDS_H

#include <bb/bank/commands.h>

#ifdef __cplusplus
extern "C" {
#else
typedef void UDPStream;
typedef void TCPStream;
typedef void TCPServer;
#endif

int BBCALL bbCallDLL( BBStr *dll,BBStr *fun,bbBank *in,bbBank *out );

#ifdef __cplusplus
}
#endif

#endif
