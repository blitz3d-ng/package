; MidHandle/ImageXHandle()/ImageYHandle()/AutoMidHandle

; Initiate Graphics Mode
Graphics 640,480,16

; Set up the image file handle as a global
Global gfxBall

; Load the image - you may need to change the location of the file
gfxBall=LoadImage ("C:Program FilesBlitz Basicsamplesall.bmp")

; Until the user presses ESC key ...
While Not KeyHit(1)
Text 0,0,"Default Image Handle for gfxBall... Press ESC ..."
Text 0,14,"X handle-" + ImageXHandle(gfxBall) ; Print the location of the image  handle x location
Text 0,28,"Y handle-" + ImageYHandle(gfxBall) ; Print the location of the image  handle y location
DrawImage gfxBall,200,200,0 ; draw the image at 200,200
Wend

; Clear the screen
Cls

; Set the ball's handle to the center of the image
MidHandle gfxBall

; Until the user presses ESC key ... show the new information
While Not KeyHit(1)
Text 0,0,"New Image Handle for gfxBall... Press ESC ..."
Text 0,14,"X handle-" + ImageXHandle(gfxBall)
Text 0,28,"Y handle-" + ImageYHandle(gfxBall)
DrawImage gfxBall,200,200,0
Wend

; Makes all images load up with their handles in the center of the image
AutoMidHandle True
Cls

; Load the image again
gfxBall=LoadImage ("C:Program FilesBlitz Basicsamplesall.bmp")

; Until the user presses ESC key ... show the new information
While Not KeyHit(1)
Text 0,0,"Automatic image handle of gfxBall... Press ESC ..."
Text 0,14,"X handle-" + ImageXHandle(gfxBall)
Text 0,28,"Y handle-" + ImageYHandle(gfxBall)
DrawImage gfxBall,200,200,0
Wend
