; ATan example

Const width = 640, height = 480

Graphics width, height


Local T#, AT#   ; Tan and ATan

zero# = 0

Print "       T      ATan( T )"
Print "======================="

; First, an extreme case...

T# = 1 / zero    ; +Infinity
AT = ATan( T ) 
Print RSet(T, 10) + RSet( AT, 11)

; Now, back to normal usage...

For n = -10 To 10

T = Sgn( n ) * n * n
AT = ATan( T ) 
Print RSet(T, 10) + RSet( AT, 11)

Next

; Finally, another extreme case.

T# = -1 / zero    ; -Infinity
AT = ATan( T ) 
Print RSet(T, 10) + RSet( AT, 11)

WaitKey()  :  End