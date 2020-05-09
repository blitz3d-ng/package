#ifndef BB_SOCKET_COMMANDS_H
#define BB_SOCKET_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C" {
#else
typedef void UDPStream;
typedef void TCPStream;
typedef void TCPServer;
#endif

int BBCALL bbCountHostIPs( BBStr *host );
int BBCALL bbHostIP( int index );

UDPStream * BBCALL bbCreateUDPStream( int port );
void BBCALL bbCloseUDPStream( UDPStream *p );
int BBCALL bbRecvUDPMsg( UDPStream *p );
void BBCALL bbSendUDPMsg( UDPStream *p,int ip,int port );
int BBCALL bbUDPStreamIP( UDPStream *p );
int BBCALL bbUDPStreamPort( UDPStream *p );
int BBCALL bbUDPMsgIP( UDPStream *p );
int BBCALL bbUDPMsgPort( UDPStream *p );
void BBCALL bbUDPTimeouts( int rt );

BBStr * BBCALL bbDottedIP( int ip );

TCPStream * BBCALL bbOpenTCPStream( BBStr *server,int port,int local_port );
void BBCALL bbCloseTCPStream( TCPStream *p );

TCPServer * BBCALL bbCreateTCPServer( int port );
void BBCALL bbCloseTCPServer( TCPServer *p );
TCPStream * BBCALL bbAcceptTCPStream( TCPServer *server );

int BBCALL bbTCPStreamIP( TCPStream *p );
int BBCALL bbTCPStreamPort( TCPStream *p );
void BBCALL bbTCPTimeouts( int rt,int at );

#ifdef __cplusplus
}
#endif

#endif
