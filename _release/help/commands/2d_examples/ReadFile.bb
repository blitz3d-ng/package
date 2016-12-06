; Reading and writing custom types to files using ReadFile, WriteFile and  CloseFile 

; Initialise some variables for the example
Type HighScore
Field Name$
Field Score%
Field Level%
End Type

Best.HighScore = New HighScore
BestName = "Mark"
BestScore = 11657
BestLevel = 34

; Open a file to write to
fileout = WriteFile("mydata.dat")

; Write the information to the file
WriteString( fileout, BestName )
WriteInt( fileout, BestScore )
WriteByte( fileout, BestLevel )

; Close the file
CloseFile( fileout )

; Open the file to Read
filein = ReadFile("mydata.dat")

; Lets read the Greatest score from the file
Greatest.HighScore = New HighScore
GreatestName$ = ReadString$( filein )
GreatestScore = ReadInt( filein )
GreatestLevel = ReadByte( filein )

; Close the file once reading is finished
CloseFile( filein )

Print "High score record read from - mydata.dat "
Print
Write "Name = "
Print GreatestName
Write "Score = "
Print GreatestScore
Write "Level = "
Print GreatestLevel

WaitKey()