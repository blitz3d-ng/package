; CreateTCPServer, CloseTCPServer, AcceptTCPStream Example
; This code is in two parts, and needs to be run seperately on the same machine

; --- Start first code set ---
; Create a server and listen for push

svrGame=CreateTCPServer(8080)

If svrGame<>0 Then 
Print "Server started successfully."
Else
Print "Server failed to start."
End
End If

While Not KeyHit(1)
strStream=AcceptTCPStream(svrGame)
If strStream Then 
Print ReadString$(strStream)
Delay 2000
End
Else 
Print "No word from Apollo X yet ..."
Delay 1000
End If 
Wend

End

; --- End first code set ---


; --- Start second code set ---
; Copy this code to another instance of Blitz Basic
; Run the above code first, then run this ... they will 'talk'

; Create a Client and push data

strmGame=OpenTCPStream("127.0.0.1",8080)

If strmGame<>0 Then 
Print "Client Connected successfully."
Else
Print "Server failed to connect."
WaitKey 
End
End If

; write stream to server
WriteString strmGame,"Mission Control, this is Apollo X ..."
Print "Completed sending message to Mission control..."

; --- End second code set ---
