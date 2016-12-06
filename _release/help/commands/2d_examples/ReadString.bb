; Reading and writing a file using ReadString$ and WriteString functions

; Initialise some variables for the example
String1$ = "A short string"
String2$ = "A longer string since these are variables lengths"
String3$ = "This is string3 "
String4$ = "joined to string4"

; Open a file to write to
fileout = WriteFile("mydata.dat")

; Write the information to the file
WriteString( fileout, String1 )
WriteString( fileout, String2 )
WriteString( fileout, String3 + String4)
WriteString( fileout, "Just to show you don't have to use variables" )

; Close the file
CloseFile( fileout )
; Open the file to Read
filein = ReadFile("mydata.dat")

Read1$ = ReadString$( filein )
Read2$ = ReadString$( filein )
Read3$ = ReadString$( filein )
Read4$ = ReadString$( filein )

; Close the file once reading is finished
CloseFile( filein )

Print "String Variables Read From File - mydata.dat "
Print
Print Read1
Print Read2
Print Read3
Print Read4

WaitKey()