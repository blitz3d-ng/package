
Context "Multiplay"

Expect HostNetGame( "Suite Test" )<>0,"Game is created"

player=CreateNetPlayer( "Server" )
SendNetMsg 1,"Hello, world!",player,0

RecvNetMsg()
