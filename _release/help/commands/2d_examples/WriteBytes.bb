; Read/WriteBytes Commands Example

; Create a 50 byte memory bank
bnkTest=CreateBank(500)

; Let's fill the bank with random data
For t = 1 To 50

	PokeByte bnkTest,t,Rnd(255)
	
Next

; Open a file to write to
fileBank=WriteFile("test.bnk")
; Write the bank to the file
WriteBytes bnkTest,fileBank,0,50
; Close it
CloseFile fileBank

; Free the bank
FreeBank bnkTest

; Make a new one
bnkTest=CreateBank(500)

; Open the file to read from
fileBank=OpenFile("test.bnk")
; Write the bank to the file
ReadBytes bnkTest,fileBank,0,50
; Close it
CloseFile fileBank

; Write back the results!
For t = 1 To 50
	
	Print PeekByte (bnkTest,t)

Next