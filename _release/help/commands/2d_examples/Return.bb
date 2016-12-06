; RETURN example

; Set result to the return value of the function 'testme'
result=testme(Rnd(0,10));

; The program effectively ends here.

; The actual function
Function testme(test);

; If the random number passed = 0 
If test=0 Then
Print "Value was 0"
Return False ; The Function ends immediately
Else
Print "The value was greater than 0"
Return True ; The Function ends immediately
End If
Print "This line never gets printed!"
End Function