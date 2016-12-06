; stopNetGame() example

newGame = StartNetGame()
; Check the status of the new game.
If newGame = 0 Then
print "Could not start or join net game."
ElseIf newGame = 1
print "Successfully joined the network game."
ElseIf newGame = 2
print "A new network game was started."
EndIf
waitkey()
StopNetGame()
print "The Network game was stopped."
