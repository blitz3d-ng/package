StartNetGame%():"bbStartNetGame"
HostNetGame%(game_name$):"bbHostNetGame"
JoinNetGame%(game_name$,ip_address$):"bbJoinNetGame"
StopNetGame():"bbStopNetGame"

CreateNetPlayer%(name$):"bbCreateNetPlayer"
DeleteNetPlayer(player%):"bbDeleteNetPlayer"
NetPlayerName$(player%):"bbNetPlayerName"
NetPlayerLocal%(player%):"bbNetPlayerLocal"

SendNetMsg%(type%,msg$,from_player%,to_player%=0,reliable%=1):"bbSendNetMsg"

RecvNetMsg%():"bbRecvNetMsg"
NetMsgType%():"bbNetMsgType"
NetMsgFrom%():"bbNetMsgFrom"
NetMsgTo%():"bbNetMsgTo"
NetMsgData$():"bbNetMsgData"
