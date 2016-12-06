; Changing part of a file using OpenFile, SeekFile, FilePos

; Open/create a file to Write
fileout = WriteFile("mydata.dat")

; Write the information to the file
WriteInt( fileout, 100 )
WriteInt( fileout, 200 )
WriteInt( fileout, 300 )
WriteInt( fileout, 400 )
WriteInt( fileout, 500 )

; Close the file
CloseFile( fileout )

DisplayFile( "The file as originally written", mydata.dat" )

Print "Data read in random order"
; Open the file to read just the 4th and 2nd elements from

file = OpenFile("mydata.dat")

; read and print the 4th integer ie 4*4-4 = 12 byte from the start of the file
SeekFile( file, 12 ) ; Move to the found location
Number = ReadInt( file )
Print Number

; read and print the 2th integer ie 2*4-4 = 4 bytes from the start of the file
SeekFile( file, 4 ) ; Move to the found location
Number = ReadInt( file )
Print Number

CloseFile( file )

Waitkey()
End ; End of program

; **** Function Definitions follow ****
; Read the file and print it
Function DisplayFile( Tittle$, Filename$ )
Print tittle$
file = ReadFile( Filename$ )
While Not Eof( file )
Number = ReadInt( file )
Print Number
Wend
CloseFile( file )
Print
End Function