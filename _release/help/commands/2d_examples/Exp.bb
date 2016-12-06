; Exp example

; Exp(x) is the same as e^x.

; Actually, due to the approximate nature of floating point arithmetic
; these will not be exactly equal. But in the following example
; the difference is so small you can't see it.

e# = 2.71828182845905  ; over-specified, might help and can't hurt.

x# = .125  ; = 1/8

Print "   x        Exp(x)       e^x  "
Print "=======   =========   ========="

For k = 1 To 7

Write " " + LSet( x , 7 )
Write RSet( Exp( x ), 10 )
Print RSet( e^x , 12 ) 
x = 2 * x

Next

WaitKey() : End