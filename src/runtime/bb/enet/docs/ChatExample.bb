
Graphics 800,600,32,2
SetBuffer BackBuffer()

SetFont LoadFont("courier new bold",12*2)

Type Message
	Field from$
	Field content$
End Type

If HostNetGame( "Chat Example" )
	WriteStdout "Created server."+Chr(10)
	player=CreateNetPlayer( "Server" )
ElseIf JoinNetGame( "","" )
	WriteStdout "Created client."+Chr(10)
	player=CreateNetPlayer( "Client" )
Else
	RuntimeError "Could not create game or connect to game."
EndIf

SendNetMsg 1,"Hello, world!",player,0

While Not KeyDown(1)
	While RecvNetMsg()
		WriteStdout "New Message!"
		Select NetMsgType()
		Case 1
			m.Message = New Message
			m\from="Player";NetPlayerName( NetMsgFrom() )
			m\content=NetMsgData()
		End Select
	Wend

	Cls
	i = 0
	For m.Message = Each Message
		Text 0,i*FontHeight(),m\from+": "+m\content
		i = i + 1
	Next
	Flip
Wend
