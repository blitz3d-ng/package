;gamma demo - use left/right arrows to
;control display intensity
;
;hold down left-control for a 'red-out' effect!

Graphics 640,480,16,1
SetBuffer BackBuffer()

n=0

While Not KeyHit(1)

If KeyDown(203) n=n-1
If KeyDown(205) n=n+1

If KeyDown(29)
SetGammaRed(n)
Else
SetGammaIntensity(n)
EndIf
Cls
SeedRnd 1234
For k=1 To 1000
Color Rnd(255),Rnd(255),Rnd(255)
Rect Rnd(640),Rnd(480),Rnd(64),Rnd(64)
Next

Text 0,0,"Intensity offset="+n

Flip

Wend

End

Function SetGammaRed( n )

For k=0 To 255
SetGamma k,k,k,k+n,0,0
Next

UpdateGamma

End Function

Function SetGammaIntensity( n )

For k=0 To 255
SetGamma k,k,k,k+n,k+n,k+n
Next

UpdateGamma

End Function