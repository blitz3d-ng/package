// AUTOGENERATED. DO NOT EDIT.
// RUN `make` TO UPDATE.

#include <bb/blitz/module.h>
#include <bb/multiplay.enet/multiplay.enet.h>

BBMODULE_LINK( multiplay_enet ){
	rtSym( "%HostNetGame$game_name",bbHostNetGame );
	rtSym( "%JoinNetGame$game_name$ip_address",bbJoinNetGame );
	rtSym( "%CreateNetPlayer$name",bbCreateNetPlayer );
	rtSym( "%SendNetMsg%type$msg%from_player%to_player=0%reliable=1",bbSendNetMsg );
	rtSym( "%RecvNetMsg",bbRecvNetMsg );
	rtSym( "%NetMsgType",bbNetMsgType );
	rtSym( "%NetMsgFrom",bbNetMsgFrom );
	rtSym( "%NetMsgTo",bbNetMsgTo );
	rtSym( "$NetMsgData",bbNetMsgData );
}