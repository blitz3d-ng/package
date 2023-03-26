REM A quick script.
@echo off

IF "%Platform%" == "x64" (
  set BLITZCC=blitzcc64.exe
) ELSE (
  set BLITZCC=blitzcc.exe
)

echo Running suite for %Platform%...

_release\bin\%BLITZCC% -r test test\all.bb || exit /b 1
IF %ERRORLEVEL% NEQ 0 (
  EXIT /B %ERRORLEVEL%
)

IF "%Platform%" == "x86" (
  _release\bin\%BLITZCC% -c -r test -o suite.exe test\all.bb
  IF %ERRORLEVEL% NEQ 0 (
    EXIT /B %ERRORLEVEL%
  )
)

echo Success
