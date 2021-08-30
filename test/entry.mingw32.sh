#/bin/sh

set -e

export blitzpath=_release/toolchains/mingw32

# compile an executable...
wine _release/toolchains/mingw32/bin/blitzcc.exe -r test -o blank.exe test/blank.bb
#wine test.exe
