; Reading and writing a file using ReadLine$ and WriteLine functions

; Initialise some variables for the example
String1$ = "Line 1 is short"
String2$ = "Line 2 is a longer line but they can be much longer"
String3$ = "Line 3 is made up "
String4$ = "of two parts joined together."

; Open a file to write to
fileout = WriteFile("mydata.txt")

; Write the information to the file
WriteLine( fileout, String1 )
WriteLine( fileout, String2 )
WriteLine( fileout, String3 + String4)
WriteLine( fileout, "Just to show you don't have to use variables" )

; Close the file
CloseFile( fileout )

; Open the file to Read
filein = ReadFile("mydata.txt")

Read1$ = ReadLine( filein )
Read2$ = ReadLine$( filein )
Read3$ = ReadLine$( filein )
Read4$ = ReadLine$( filein )

; Close the file once reading is finished
CloseFile( filein )

Print "Lines of text read from file - mydata.txt "
Print
Print Read1
Print Read2
Print Read3
Print Read4

WaitKey()
