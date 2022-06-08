#/bin/sh

set -e

export blitzpath=_release/toolchains/mingw32
cp deps/fmod/bin/fmod.dll _release/toolchains/mingw32/bin

# compile an executable...
wine _release/toolchains/mingw32/bin/blitzcc.exe -r test -o blank.exe test/blank.bb
cp deps/fmod/bin/fmod.dll test/
wine test/blank.exe

# run the specs...
wine _release/toolchains/mingw32/bin/blitzcc.exe -r test test/all.bb