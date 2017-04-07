
#ifndef MULTIPLAY_H
#define MULTIPLAY_H

#include "bbsys.h"
#include <dplay.h>

void multiplay_link();
bool multiplay_create();
bool multiplay_destroy();

int		 BBCALL bbStartNetGame();
int		 BBCALL bbHostNetGame( BBStr *name );
int		 BBCALL bbJoinNetGame( BBStr *name,BBStr *address );
void	 BBCALL bbStopNetGame();

DPID	 BBCALL bbCreateNetPlayer( BBStr *name );
void	 BBCALL bbDeleteNetPlayer( DPID player );
BBStr *	 BBCALL bbNetPlayerName( DPID player );
int		 BBCALL bbNetPlayerLocal( DPID player );

int		 BBCALL bbSendNetMsg( int type,BBStr *msg,DPID from,DPID to,int reliable );

int		 BBCALL bbRecvNetMsg();
int		 BBCALL bbNetMsgType();
BBStr *	BBCALL bbNetMsgData();
DPID	 BBCALL bbNetMsgFrom();
DPID	 BBCALL bbNetMsgTo();

#endif
