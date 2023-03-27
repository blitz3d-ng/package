REM A quick script.
@echo off

IF "%1" == "clean" (
  RMDIR /s /q build\
  RMDIR /s /q _release\bin
  EXIT /B
)

set BBENV=%1

IF "%Platform%" == "x86" (
  set BBARCH=x86
  set BBPLATFORM=win32
) ELSE (
  IF "%Platform%" == "x64" (
    set BBARCH=x86_64
    set BBPLATFORM=win64
  ) ELSE (
    echo Unsupport platform %Platform%.
    exit /b 1
  )
)

IF NOT "%BBENV%" == "debug" (
  IF NOT "%BBENV%" == "release" (
    SET BBENV=release
  )
)

set BBBUILDDIR=%cd%\build\%BBPLATFORM%-%BBENV%

cmake -G Ninja -H. -B%BBBUILDDIR% -DARCH=%BBARCH% -DBB_PLATFORM=%BBPLATFORM% -DBB_ENV=%BBENV% || exit /b 1
cmake --build %BBBUILDDIR% || exit /b 1
