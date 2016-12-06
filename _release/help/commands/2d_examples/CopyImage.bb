; CopyImage Example

; Load an image and give its handle to gfxOld variable
gfxOld=LoadImage("mypicture.bmp")

; Duplicate the gfxOld image to a new handle variable
gfxNew=CopyImage(gfxOld)
