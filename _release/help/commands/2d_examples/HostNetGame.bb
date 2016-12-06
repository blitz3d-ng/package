; HostNetGame example

joinResults=HostNetGame("ShaneGame")

Select joinResults
Case 2
Print "Successfully started host game!"
Default
Print "Game was unable to start!"
End Select 
waitkey()
