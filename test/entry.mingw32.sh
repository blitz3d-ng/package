#/bin/sh

set -e

export blitzpath=_release/toolchains/mingw32
cp deps/fmod/bin/fmod.dll _release/toolchains/mingw32/bin

# compile an executable...
wine _release/toolchains/mingw32/bin/blitzcc.exe -r test -o blank.exe test/blank.bb
wine test.exe
