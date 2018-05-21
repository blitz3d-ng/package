#ifndef BB_ENET_COMMANDS_H
#define BB_ENET_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C" {
#else
typedef void BBPlayer;
#endif

bb_int_t	BBCALL bbStartNetGame();
bb_int_t	BBCALL bbHostNetGame( BBStr *name );
bb_int_t	BBCALL bbJoinNetGame( BBStr *name,BBStr *address );
void	 BBCALL bbStopNetGame();

BBPlayer * BBCALL bbCreateNetPlayer( BBStr *name );
void	 BBCALL bbDeleteNetPlayer( BBPlayer *player );
BBStr *	 BBCALL bbNetPlayerName( BBPlayer *player );
bb_int_t	BBCALL bbNetPlayerLocal( BBPlayer *player );

bb_int_t	BBCALL bbSendNetMsg( bb_int_t type,BBStr *msg,BBPlayer *from,BBPlayer *to,bb_int_t reliable );

bb_int_t	BBCALL bbRecvNetMsg();
bb_int_t	BBCALL bbNetMsgType();
BBStr *	BBCALL bbNetMsgData();
BBPlayer * BBCALL bbNetMsgFrom();
BBPlayer * BBCALL bbNetMsgTo();

#ifdef __cplusplus
}
#endif

#endif
