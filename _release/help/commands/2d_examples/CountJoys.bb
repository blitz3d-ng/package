
Graphics 800,600

Print "Here is a list of joysticks attached to your system:"
For i=0 To CountJoys()-1
	Print i+": "+JoyName(i)+" ("+JoyID(i)+")"
Next

WaitKey