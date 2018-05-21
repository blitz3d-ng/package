; DPI example

Graphics 800,600,32,2
SetFont LoadFont( "blitz",12 )

If DPIScaleX() = 1.0 And DPIScaleY() = 1.0
  Print "Looks like you're on a 96dpi monitor! Nothing to see here, unfortunately!"
  WaitKey
  End
EndIf


Print "Your DPI scale is " + DPIScaleX() + "x" + DPIScaleY()
Print "This is a ~qnative~q 800x600 window. The text is quite small!"
Print "Press any key to see a more properly scaled window."
WaitKey

EndGraphics

Graphics 800*DPIScaleX(),600*DPIScaleY(),32,2
SetFont LoadFont( "blitz",12*DPIScaleY() )

Print "Ah, much better."

WaitKey
