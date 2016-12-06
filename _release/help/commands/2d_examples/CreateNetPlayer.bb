; CreateNetPlayer example

newGame = StartNetGame()
; Check the status of the new game.
If newGame = 0 Then
Print "Could not start or join net game."
End

ElseIf newGame = 1
Print "Successfully joined the network game"
ElseIf newGame = 2
Print "A new network game was started!"
EndIf

; Create a random player name
name$="Player" + Rand(100)

; Get a unique player id number for the player
; and create the player
playerID=CreateNetPlayer(name$)

If playerID = 0 Then 
Print "Player could not be created!"
Else
Print "Player " + name$ + " was created and given ID#" + playerID
End If
WaitKey()
