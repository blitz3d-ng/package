;StartNetGame%():"bbStartNetGame"
HostNetGame%( game_name$ ):"bbHostNetGame"
JoinNetGame%( game_name$,ip_address$ ):"bbJoinNetGame"
;StopNetGame():"bbStopNetGame"

CreateNetPlayer.BBPlayer( name$ ):"bbCreateNetPlayer"
;DeleteNetPlayer( player.BBPlayer ):"bbDeleteNetPlayer"
;NetPlayerName$( player.BBPlayer ):"bbNetPlayerName"
;NetPlayerLocal%( player.BBPlayer ):"bbNetPlayerLocal"

SendNetMsg%( type%,msg$,from_player.BBPlayer,to_player.BBPlayer=0,reliable%=1 ):"bbSendNetMsg"

RecvNetMsg%():"bbRecvNetMsg"
NetMsgType%():"bbNetMsgType"
NetMsgFrom.BBPlayer():"bbNetMsgFrom"
NetMsgTo.BBPlayer():"bbNetMsgTo"
NetMsgData$():"bbNetMsgData"
