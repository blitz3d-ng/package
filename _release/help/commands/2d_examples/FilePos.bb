; Changing part of a file using OpenFile, SeekFile, FilePos
; note FilePos is used in the SearchFile function at the end of this example

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
Position = SearchFile( 4 , "mydata.dat" )
Write "Value 4 was found "
Write Position
Print " bytes from the start."
Print

; Open the file and change the value 3 to 9999

file = OpenFile("mydata.dat")
SeekFile( file, Position ) ; Move to the found location
WriteInt( file, 9999 ) ; Replace the original value with 9999
CloseFile( file )


DisplayFile( "The file after being modified", "mydata.dat" )
WaitKey()
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

; Search a file of integers for the Wanted data value
; Note the need to subtract 4 from the location since having read it
; we are now pointing at the next Integer also Return() was placed
; after closing the file so it is closed properly
Function SearchFile( Wanted, Filename$ )
file = ReadFile( Filename$ )
While Not Eof( file )
If ReadInt( file ) = Wanted Then Location = FilePos( file ) - 4
Wend
CloseFile( file )
Return( Location )
End Function