; Reading and writing a file using ReadShort and WriteShort functions

; Initialise some variables for the example
Int1% = 10 ; store 10
Int2% = 365 ; store 365
Int3% = 32767 ; 32767 is the largest positive Short Integer Value in BlitzBasic  )
Int4% = -32768 ; -32768 the largest negative Short Integer Value in BlitzBasic  )

; Open a file to write to
fileout = WriteFile("mydata.dat")

; Write the information to the file
WriteShort( fileout, Int1 )
WriteShort( fileout, Int2 )
WriteShort( fileout, Int3 )
WriteShort( fileout, Int4 )

; Close the file
CloseFile( fileout )

; Open the file to Read
filein = ReadFile("mydata.dat")

Read1 = ReadShort( filein )
Read2 = ReadShort( filein )
Read3 = ReadShort( filein )
Read4 = ReadShort( filein )

; Close the file once reading is finished
CloseFile( filein )

Print "Short Integer Data Read From File - mydata.dat "
Print Read1
Print Read2
Print Read3
Print Read4

WaitKey()
