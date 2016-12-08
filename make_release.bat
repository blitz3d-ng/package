@echo off
echo "Building x86..."
premake5 vs2015 && devenv build\blitz3d.sln /rebuild "release|win32" || exit /b
rem echo "Building x64..."
rem premake5 vs2015 && devenv build\blitz3d.sln /rebuild "release|win64" || exit /b

echo "Compiling mediaview..."
set blitzpath=_release
_release\bin\blitzcc.exe -o _release\bin\mediaview.exe _release\mediaview\mediaview.bb

echo "Removing intermediate files..."
del /F _release\bin\.vs
del /Q _release\bin\*.ilk _release\bin\*.pcb _release\bin\*.pdb _release\bin\*.lib _release\bin\*.exp
del /Q _release\*.ilk _release\*.pcb _release\*.pdb
del /F _release\cfg\blitzide.prefs
del /F _release\tmp
del /F _release\cfg\blitzide.prefs
