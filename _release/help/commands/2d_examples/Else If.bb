; ELSEIF Example

; Input the user's name
name$=Input$("What is your name? ")

; Doesn't the person's name equal SHANE?
If name$ = "Shane" Then
	Print "You are recognized, Shane! Welcome!"

ElseIf name$="Ron" Then
	Print "You are recognized too, Ron! Welcome!"

Else
	Print "Sorry, you don't belong here!"

; End of the condition checking
End If