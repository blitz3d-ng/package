REM @echo off

SET blitzpath=_release
COPY deps\fmod\bin\fmod.dll test\

_release\bin\blitzcc.exe -r test -o blank.exe test\blank.bb
test\blank.exe
