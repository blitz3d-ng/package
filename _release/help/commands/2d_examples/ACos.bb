; ACos example

; NaN means "Not a Number", the numerical result is invalid.

Const width = 640, height = 480

Graphics width, height


Local C#, AC#   ; Cos and ACos

Print "    C    ACos( C )"
Print "=================="

For n = -11 To 11

C = n / 10.0
AC = ACos( C ) 
Print RSet(C, 6) + RSet( AC, 10)

Next

WaitKey()  :  End