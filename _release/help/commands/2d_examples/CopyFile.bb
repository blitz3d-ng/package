file$="c:autoexec.bat"
destination$="a:autoexec.bat"

Print "Press any key to copy your Autoexec.bat file to floppy"

WaitKey()

CopyFile file$,destination$