; NetMsgData$() example
; --------------------
; Run this program on the REMOTE computer to 'watch'
; the activity of the SendNetMsg example. Run that
; example on local machine.
;
; This program will tell you when a player involved in
; the game hits a wall ...

; We'll use this instead of JoinHostGame - make it easier
StartNetGame()

; Create a player - a player must be created to
; receive mesages!
playerID=CreateNetPlayer("Shane")

; Loop and get status
While Not KeyHit(1)

; Check to see if we've received a message
If RecvNetMsg() Then

; if we did, let's figure out what type it is
; we know it will be a user message, though
msgType=NetMsgType()

; 1-99 means a user message
If msgType>0 And msgType<100 Then

; Let's see who the message was from
msgFrom=NetMsgFrom()

; Let's get the message!
msgData$=NetMsgData$()

; Print the message
Print msgData$
Print "(Message was to:"+ NetMsgTo() + ")"
End If
End If
Wend
