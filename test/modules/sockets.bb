Context "Sockets"

ExpectStr DottedIP(2130706433), "127.0.0.1"

ips=CountHostIPs("")
Expect ips>0,"At least one IP for localhost"

ifaces=CountNetInterfaces()
Expect ifaces>0,"At least one network interface"
Expect NetInterfaceName(1)<>"","Iface has a name"
Expect NetInterfaceIP(1)<>"","Iface has an IP"

; TCP
TCPTimeouts 1000,1000

tcp=OpenTCPStream( "github.com",80,8080 )
Expect tcp, "Opened TCP stream"
Expect TCPStreamIP(tcp)<>0,"TCP stream has IP"
ExpectInt TCPStreamPort(tcp),80,"TCP stream is on port 80"

WriteLine tcp,"GET / HTTP/1.0"
WriteLine tcp,""

res$=""
While Not Eof(tcp)
  res=res+ReadLine$( tcp )+Chr(10)
Wend
Expect Instr( res, "HTTP/1.1 301 Moved Permanently")>0,"Expect 301 response"

CloseTCPStream tcp

SeedRnd MilliSecs()
server_port=9090+Rand(0, 100) ; a little dangerous, but avoid issues in dev

tcp_server=CreateTCPServer( server_port )
Expect tcp_server,"Creates a TCP server on port "+server_port

tcp_out=OpenTCPStream( "127.0.0.1",server_port )
Expect tcp_out,"Opens a TCP connection to the server"
WriteLine tcp_out,"hello, world"

tcp_in=AcceptTCPStream(tcp_server)
Expect tcp_in,"Accepts the TCP connection"
ExpectStr ReadLine(tcp_in),"hello, world"
ExpectInt ReadAvail(tcp_in),0,"There are no bytes to read"

CloseTCPServer tcp_server

; UDP
UDPTimeouts 1000

udp_out=CreateUDPStream( 8080 )
Expect udp_out<>0,"Opened outgoing UDP socket"
ExpectInt UDPStreamIP(udp_out),0
ExpectInt UDPStreamPort(udp_out),8080

udp_in=CreateUDPStream( 8081 )
Expect udp_in<>0,"Opened incoming UDP socket"
ExpectInt UDPStreamIP(udp_in),0
ExpectInt UDPStreamPort(udp_in),8081,"Incoming socket is on port 8081"

For i=0 To 9 ; send a bunch just to be safe...
  WriteString udp_out,"hello world"
  SendUDPMsg udp_out,HostIP(1),8081
Next

ExpectInt RecvUDPMsg(udp_in),HostIP(1),"Receives a message"
ExpectInt UDPMsgIP(udp_in),HostIP(1)
ExpectInt UDPMsgPort(udp_in),8080

Expect ReadAvail(udp_in)>0,"There is message content"
ExpectStr ReadString(udp_in),"hello world"
Expect Eof(udp_in),"udp_in EOF"

CloseUDPStream udp_in
CloseUDPStream udp_out
