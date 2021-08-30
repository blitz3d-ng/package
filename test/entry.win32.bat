REM @echo off

SET blitzpath=_release

_release\bin\blitzcc.exe -r test -o blank.exe test\blank.bb
blank.exe
