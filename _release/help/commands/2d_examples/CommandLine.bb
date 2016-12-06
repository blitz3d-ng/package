; CommandLine$() Example
; Be sure to use PROGRAM->PROGRAM COMMAND LINE from the
; pull down and put /debug in there to test with.

a$=CommandLine$()

If a$="/debug" Then 
Print "Debug mode is on!"
debug=1
Else
Print "No debugging activated."
debug=0
End If