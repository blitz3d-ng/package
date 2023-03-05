#/bin/sh

set -e

export blitzpath=_release
cp deps/fmod/bin/fmod*.dll _release/bin

BLITZCC="wine _release/bin/blitzcc.exe"

# compile an executable...
$BLITZCC -r test -o blank.exe test/blank.bb
cp deps/fmod/bin/fmod.dll test/
wine test/blank.exe

# run the specs...
$BLITZCC -r test test/all.bb
