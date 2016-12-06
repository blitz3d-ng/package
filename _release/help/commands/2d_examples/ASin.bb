; ASin example

; NaN means "Not a Number", the numerical result is invalid.

Const width = 640, height = 480

Graphics width, height


Local S#, AS#   ; Sin and ASin

Print "    S    ASin( S )"
Print "=================="

For n = -11 To 11

S = n / 10.0
AS = ASin( S ) 
Print RSet(S, 6) + RSet( AS, 10)

Next

WaitKey()  :  End