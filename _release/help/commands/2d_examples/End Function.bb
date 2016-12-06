; End Function Example

; Get the user's name
name$=Input$("Enter Your Name:")

; Call a function to print how many letters the name has
numletters(name$);

;The program basically ends here, because functions don't run unless called.

; The actual function
Function numletters(passedname$)
Print "Your name has " + Len(passedname$) + " letters in it."
End Function
