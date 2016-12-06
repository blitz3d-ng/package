; CountGFXModes()/GfxModeWidth/GfxModeHeight/GfxModeDepth example

intModes=CountGfxModes()

Print "There are " + intModes + "graphic modes available:"

; Display all modes including width, height, and color depth
For t = 1 To intModes
Print "Mode " + t + ":
Print "Width=" + GfxModeWidth(t)
Print "Height=" + GfxModeHeight(t)
Print "Height=" + GfxModeDepth(t)
Next
