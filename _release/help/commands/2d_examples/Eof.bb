; Eof sample

file$="c:autoexec.bat"

filein = ReadFile(file$)

Print "Here is your Autoexec.bat file ..."

; Loop this until we reach the end of file
While Not Eof(filein)
Print ReadLine$(filein)
Wend
