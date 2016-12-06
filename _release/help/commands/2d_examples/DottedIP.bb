; First call CountHostIPs (blank infers the local machine)
n = CountHostIPs("")
; n now contains the total number of known host machines.

; Obtain the internal id for the IP address
ip = HostIP(1)

; Convert it to human readable IP address
ipaddress$ = DottedIP$(ip)

Print "Dotted IP Test"
Print "=============="
Print ""
Print "Internal Host IP ID:" + ip
Print "Dotted IP Address:" + ipaddress$
Print ""
Print "Press any key to continue"

WaitKey()

End