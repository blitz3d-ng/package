; OpenTCPStream/CloseTCPStream/ReadAvail Example

Print "Connecting..."
tcp=OpenTCPStream( "www.blitzbasement.com",80 )

If Not tcp Print "Failed.":WaitKey:End

Print "Connected! Sending request..."

WriteLine tcp,"GET http://www.blitzbasement.com HTTP/1.0"
WriteLine tcp,Chr$(10)

If Eof(tcp) Print "Failed.":WaitKey:End

Print "Request sent! Waiting for reply..."

While Not Eof(tcp)
Print ReadLine$( tcp )
Print "Bytes available:" + ReadAvail(tcp)
Wend

If Eof(tcp)=1 Then Print "Success!" Else Print "Error!"

CloseTCPStream tcp

WaitKey
