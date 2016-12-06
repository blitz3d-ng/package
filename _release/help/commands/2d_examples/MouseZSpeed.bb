Graphics 640, 480, 0, 2
SetBuffer BackBuffer ()

Repeat

Cls

Select MouseZSpeed ()
Case -1
result$ = "Backwards"
Case 0
; result$ = "No movement"
Case 1
result$ = "Forwards"
End Select

Text 20, 10, "NOTE: MouseZSpeed () = 0 is not listed here, to avoid confusion!"
Text 20, 40, result$
Flip

Until KeyHit (1)

End
