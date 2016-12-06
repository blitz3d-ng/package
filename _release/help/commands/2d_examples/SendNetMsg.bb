; SendNetMsg example
; ------------------
; Run this example on the local computer
; run the example for RecvNetMsg() on a remote computer

; Graphics mode with double buffering
Graphics 640,480,16
SetBuffer BackBuffer()

; Create a network game with NO requester
joinStatus=HostNetGame("ShaneGame")

; A type to hold all the player's information
Type multi
Field x
Field y
Field id
Field name$
Field xspeed
Field boxColor
End Type

; make sure the game started ok...
If joinStatus=2 Then
Print "Hosted game started... "
Else
Print "Hosted game could not be started!"
End
End If

; Create 5 local players using TYPEs
For t = 1 To 5
; New type instance
player.multi = New Multi
; Assign the ID field with the created player ID and name him
playerID=CreateNetPlayer("Player" + t)

; if the player was created ok ... assign some random parameters
If playerID <> 0 Then 
player
ame$="Player" + t
playerx = Rand(640)
playery = Rand(480)
playeroxColor = Rand(255)
playerxspeed = Rand(1,5)
; Print some text results
Print "Player " + t + " has joined the game with ID=" + playerID
Else
Print "The player couldn't join! Aborting!"
End If
Next

; We've got them all! Wait for a key
Print "All local players are joined! Press a key ..."
WaitKey()

; Loop this routine
While Not KeyHit(1)
Cls
; for each of the players, update their locations on the screen
For player = Each multi
Color playeroxColor,playeroxColor,playeroxColor
Rect playerx,playery,10,10,1
Text playerx-10,playery-15,player
ame$
playerx = playerx + playerxspeed
If playerx > 640 Or playerx < 0 Then 
playerxspeed=-playerxspeed
message$="Player ID #" + playerID + " hit a wall!"
; Send a broadcast message if a player rebounds off the wall
; this message will show up on the remote machine
SendNetMsg Rand(1,99),message$,playerid,0
End If
Next
Flip
Wend
End
