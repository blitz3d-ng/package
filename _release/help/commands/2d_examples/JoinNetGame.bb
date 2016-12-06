; JoinNetGame example
; Note; run the HostNetGame example code on the other computer
; you wish to join with

gamename$="ShaneGame"
; Change this to match the other computer's IP!
serverIP$="0.0.0.0"

; Make the join attempt
joinResults=JoinNetGame(gamename$,serverIP$)

Select joinResults
Case 1
Print "Joined the game successfully!"
Default
Print "Joining the game was unsuccessful."
End Select
WaitKey()
