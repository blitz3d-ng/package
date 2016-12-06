; Function Example

; Get the user's name
name$=Input$("Enter Your Name:")

; Call a function to print how many letters the name has
numletters(name$);

; Let's get something BACK from the function
thefirst$=firstletter(name$)

; Now print results
Print "Was the first letter an 'S'? (1=True/0=False)" + thefirst$

;The program basically ends here, because functions don't run unless called.

; The actual function
Function numletters(passedname$)
Print "Your name has " + Len(passedname$) + " letters in it."
End Function

; Function to see if the first letter is S
Function firstletter(passedname$)

; If the first letter is an 'S' then return from the function a true value
If Left$(passedname$,1) = "S" Then
Return True

; Otherwise, return false
Else

Return False 

End If 
End Function