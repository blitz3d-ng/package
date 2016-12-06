; Windows 9x users will need to change location of calc.exe

filename$="c:winntsystem32calc.exe"

if fileType(filename$)=1 then Print "The file exists!"
if fileType(filename$)=0 then Print "File not found!"
if fileType(filename$)=2 then Print "This is a directory!"

Print "Press any key to quit."

WaitKey()