; Movie Commands Example
; ======================
; This demonstrates the following commands:
;
;	OpenMovie
;	MovieHeight
;	MovieWidth
;	MoviePlaying
;	DrawMovie

; Some constants to start with
Const WIDTH = 640
Const HEIGHT = 480

; First of all, set up the graphics
Graphics WIDTH, HEIGHT
SetBuffer BackBuffer()
ClsColor 0,0,0
Color 0,255,0

; Next, open the movie file.  Feel free to change this to an AVI or MPEG file.
movie=OpenMovie("media/hat.gif")

; check to see if it loaded okay
If movie=0 Then RuntimeError "Error - Movie not loaded!"
If Not(MoviePlaying(movie)) Then RuntimeError "Error - Movie not playing!"

;Now determine the size of the movie
w=MovieWidth(movie)     ; the width of the movie
h=MovieHeight(movie)    ; the height of the movie

; Now set up the starting position and timing variables
x=(WIDTH-w)/2           ; the x position of the movie on screen
y=(HEIGHT-h-100)/2      ; the y position of the movie on screen
period=100              ; the interval between frames
time=MilliSecs()        ; the time of the last frame update

; And here's the main loop
Repeat

; Wait for the specified period

; GIFs have no timing info, and as such will redraw the next frame on each call to DrawMovie.
; AVIs and MPEGs do have timing info, and as such will redraw the most recent frame on each call to DrawMovie.
; Ergo, this time limiter only has an impact, and is only required for GIFs.
Repeat
; do nothing
Until MilliSecs()-time>=period
time=MilliSecs()    ; save the current time for the next frame

; Handle keyboard inputs

; CONTROL adjusts the speed with which we do stuff	
If KeyDown(29) Or KeyDown(157) Then
change=5
Else
change=1
End If

; SHIFT means we're dealing with the size
If KeyDown(42) Or KeyDown(54) Then
	If KeyDown(203) And w>change-1 Then w=w-change
	If KeyDown(205) And x+w+change < WIDTH Then w=w+change
	If KeyDown(200) And h>change-1 Then h=h-change
	If KeyDown(208) And y+h+change < HEIGHT Then h=h+change
Else
	; otherwise it's the position that we're changing
	If KeyDown(203) And x>change-1 Then x=x-change
	If KeyDown(205) And x+w+change < WIDTH Then x=x+change
	If KeyDown(200) And y>change-1 Then y=y-change
	If KeyDown(208) And y+h+change < HEIGHT Then y=y+change
EndIf

; +/- to change the animation speed
If ( KeyDown(13) Or KeyDown(78) ) And period>change Then period=period-change
If ( KeyDown(12) Or KeyDown(74) ) And period < 500 Then period=period+change

; Redraw the screen, by...

Cls                         ; clear the screen
DrawMovie movie,x,y,w,h     ; draw the movie
; draw the instructions
Text 0,0,"Use the arrow keys to reposition the movie."			
Text 0,20,"Hold SHIFT with the arrow keys to resize."					
Text 0,40,"Use + or - or control the animation speed."
Text 0,60,"Hold CONTROL to resize, move, or change speed faster."	
Text 0,80,"Press ESCAPE to exit."
Text 0,100,"Current Command Syntax: DrawMovie(movie, " + x + ","+ y + "," + w + "," + h + ")"
; Flip the buffers
Flip						

Until KeyHit(1) ; Escape to exit

; Remove the movie from memory before closing down
CloseMovie(movie)

End ; bye!