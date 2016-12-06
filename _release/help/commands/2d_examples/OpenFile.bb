; Changing part of a file using OpenFile, SeekFile and WriteInt

; Open/create a file to Write
fileout = WriteFile("mydata.dat")

; Write the information to the file
WriteInt( fileout, 1 )
WriteInt( fileout, 2 )
WriteInt( fileout, 3 )
WriteInt( fileout, 4 )
WriteInt( fileout, 5 )

; Close the file
CloseFile( fileout )

DisplayFile( "The file as originally written", mydata.dat" )
; Open the file and change the Third Integer

file = OpenFile("mydata.dat")
SeekFile( file, 8 ) ; Move to the third integer in the file
WriteInt( file, 9999 ) ; Replace the original value with 9999
CloseFile( file )

DisplayFile( "The file after being midified", "mydata.dat" )
WaitKey()
; **** Function Definitions follow ****

; Read the file and print it
Function DisplayFile( Tittle$, Filename$ )
Print tittle$
filein = ReadFile( Filename$ )
While Not Eof( filein )
Number = ReadInt( filein )
Print Number
Wend
CloseFile( filein )
Print
End Function