; CreateImage/TileImage/ImageBuffer example

; Again, we'll use globals even tho we don't need them here
; One variable for the graphic we'll create, one for a timer
Global gfxStarfield, tmrScreen

; Declare graphic mode
Graphics 640,480,16

; Create a blank image that is 320 pixels wide and 32 high with 10 frames of  32x32
gfxStarfield=CreateImage(32,32,10)

; loop through each frame of the graphic we just made
For t = 0 To 9
; Set the drawing buffer to the graphic frame so we can write on it
SetBuffer ImageBuffer(gfxStarfield,t)
; put 50 stars in the frame at random locations
For y = 1 To 50
Plot Rnd(32),Rnd(32)
Next
Next

; Double buffer mode for smooth screen drawing
SetBuffer BackBuffer()

; Loop until ESC is pressed
While Not KeyHit(1)

; Only update the screen every 300 milliseconds. Change 300 for faster or
; slower screen updates
If MilliSecs() > tmrScreen+300 Then 
Cls ; clear the screen

; Tile the screen with a random frame from our new graphic starting at
; x=0 and y=0 location.
TileImage gfxStarfield,0,0,Rnd(9)
Flip ; Flip the screen into view
tmrScreen=MilliSecs() ; reset the time
End If
Wend 
