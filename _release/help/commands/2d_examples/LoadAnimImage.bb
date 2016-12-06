; LoadAnimImage/MaskImage Example
; With animation timers

; Even though we don't have any functions, let's do variables global
; One variable will hold the handle for the graphic, one will hold the
; current frame we are displaying, and one will hold the milliseconds
; timer so we can adjust the animation speed.
Global gfxSparks, frmSparks, tmrSparks

; Standard graphic declaration and double buffering setup
Graphics 640,480,16
SetBuffer BackBuffer()

; Load the imagestrip up and denote the frames 32x32 - for a total of 3 frames
gfxSparks=LoadAnimImage("c:Program FilesBlitzBasicsamplesGraphicsspark.bmp",32,32,0,3)

; We mask the image's color pink to be the 'transparent' color - look at the
; image in your favorite editor to see more why we use masking.
MaskImage gfxSparks,255,0,255

; Loop until ESC
While Not KeyHit(1)
Cls ; Standard clear screen

; The next statment checks to see if 100 milliseconds has passes since we
; last changed frames. Change the 100 to higher and lower values to 
; make the animation faster or slower.
If MilliSecs() > tmrSparks + 100 Then
tmrSparks=MilliSecs() ; 'reset' the timer
frmSparks=( frmSparks + 1 ) Mod 3 ; increment the frame, flip to 0 if we are  out
End If 
DrawImage gfxSparks,MouseX(),MouseY(),frmSparks ; draw the image
Flip ; show the buffer
Wend