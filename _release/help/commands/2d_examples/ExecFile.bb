; ExecFile sample - RUN THIS WINDOWED!
Graphics 320,240,0,2

filename$ = Chr$(34) + "calc.exe" + Chr$(34)

Text 5,5, "press any key to run CALC.EXE!"
Flip
WaitKey

ExecFile(filename$)

Text 5,25, "Press any key to quit."
Flip
WaitKey
End