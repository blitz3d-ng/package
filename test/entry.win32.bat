REM @echo off

SET blitzpath=_release

_release\bin\blitzcc.exe -r test -o blank.exe test\blank.bb
COPY deps\fmod\bin\fmod.dll test\
test\blank.exe

_release\bin\blitzcc.exe -r test test\all.bb
