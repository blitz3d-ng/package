REM @echo off

set ENV=%1
set PLATFORM=%2

IF NOT "%ENV%" == "debug" (
  IF NOT "%ENV%" == "release" (
    SET ENV=debug
  )
)

IF "%ENV%" == "debug" (
  SET CONFIG=Debug
)
IF "%ENV%" == "release" (
  SET CONFIG=MinSizeRel
)

IF NOT "%PLATFORM%" == "win32" (
  IF NOT "%PLATFORM%" == "win64" (
    SET PLATFORM=win32
  )
)

IF "%PLATFORM%" == "win32" (
  SET GENERATOR=Visual Studio 16 2019
  SET ARCH=Win32
)

IF "%PLATFORM%" == "win64" (
  SET GENERATOR=Visual Studio 16 2019
  SET ARCH=x64
)

ECHO Building %PLATFORM% in %ENV% mode.

set OUTPUT=_release\toolchains\%PLATFORM%
set RELEASE=_release

cmake -G "%GENERATOR%" -H. -B"%cd%\build\%PLATFORM%\%ENV%" -A%ARCH% -DBB_PLATFORM=%PLATFORM% -DBB_ENV=%ENV% || exit /b
msbuild /m build\%PLATFORM%\%ENV%\Blitz3D.sln /property:Configuration=%CONFIG% /property:Platform=%ARCH% || exit /b

COPY /Y %OUTPUT%\bin\Blitz3D*.exe %RELEASE%
COPY /Y %OUTPUT%\bin\ide*.exe %RELEASE%\bin
COPY /Y %OUTPUT%\bin\blitzcc*.exe %RELEASE%\bin
COPY /Y %OUTPUT%\bin\linker*.dll %RELEASE%\bin
COPY /Y %OUTPUT%\bin\debugger*.dll %RELEASE%\bin
COPY /Y %OUTPUT%\bin\runtime*.dll %RELEASE%\bin

COPY /Y deps\fmod\bin\fmod*.dll %RELEASE%\bin
COPY /Y %OUTPUT%\bin\unit_test.dll %RELEASE%\userlibs
COPY /Y src\modules\bb\unit-test\unit_test.decls %RELEASE%\userlibs
