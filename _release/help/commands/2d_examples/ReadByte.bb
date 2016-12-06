; Reading and writing a file using ReadByte and WriteByte functions

; Initialise some variables for the example
Byte1% = 10 ; store 10
Byte2% = 100 ; store 100
Byte3% = 255 ; store 255 ( the maximum value that can be stored in a Byte)
Byte4% = 256 ; try to store 256 this will end up as 0 ( i.e. 256 - 256 = 0 )
Byte5% = 257 ; try to store 257 this will end up as 1 ( i.e. 257 - 256 = 1 )
Byte6% = -1 ; try to store -1 this will end up as 255 ( i.e. 256 -1 = 255 )
Byte7% = -2 ; try to store 256 this will end up as 254 ( i.e. 256 - 2 = 254  )
Byte8% = Asc("A") ; Store the ASCII value for the Character "A" ( i.e. 65 )

; Open a file to write to
fileout = WriteFile("mydata.dat ")

; Write the information to the file
WriteByte( fileout, Byte1 )
WriteByte( fileout, Byte2 )
WriteByte( fileout, Byte3 )
WriteByte( fileout, Byte4 )
WriteByte( fileout, Byte5 )
WriteByte( fileout, Byte6 )
WriteByte( fileout, Byte7 )
WriteByte( fileout, Byte8 )

; Close the file
CloseFile( fileout )

; Open the file to Read
filein = ReadFile("mydata.dat")

Read1 = ReadByte( filein )
Read2 = ReadByte( filein )
Read3 = ReadByte( filein )
Read4 = ReadByte( filein )
Read5 = ReadByte( filein )
Read6 = ReadByte( filein )
Read7 = ReadByte( filein )
Read8 = ReadByte( filein )

; Close the file once reading is finished
CloseFile( filein )

Print "Written - Read"
Write Byte1 + " - " : Print Read1
Write Byte2 + " - " : Print Read2
Write Byte3 + " - " : Print Read3
Write Byte4 + " - " : Print Read4
Write Byte5 + " - " : Print Read5
Write Byte6 + " - " : Print Read6
Write Byte7 + " - " : Print Read7
Write Byte8 + " - " : Print Chr$( Read8 )

WaitKey()
