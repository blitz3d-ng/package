#!/bin/sh

fail=0

RED='\033[0;31m'
GRN='\033[0;32m'
NC='\033[0m'

cleanup () {
  find . -name "*.gcda" -exec rm {} \;
  find . -name "*.gcov" -exec rm {} \;
}

run() {
  $BLITZCC $2 > /dev/null 2>&1
  RESULT=$?
  if [ $RESULT -ne 0 ]
  then
    printf "  ${RED}FAIL${NC} $1\n"
    fail=1
  else
    printf "  ${GRN}PASS${NC} $1\n"
  fi
}

compile() {
  run $1 "-c $1"
}

check_flag() {
  run $1 "$1"
}

BLITZCC=_release/bin/blitzcc

cleanup

# run the proper suite
$BLITZCC -r test test/all.bb
RESULT=$?
if [ $RESULT -ne 0 ]; then
  echo "Test suite failed. Fix it and then run the coverage generation again."
  exit 1
fi

echo "Verifying samples can compile"
compile _release/samples/AGore/BirdDemo/BirdDemo.bb
compile _release/samples/AGore/GrassDemo/Grass.bb
compile _release/samples/AGore/HeadDemo/TheHead.bb

compile _release/samples/halo/Lightmap/lightmap.bb
compile _release/samples/halo/MeshFX/meshfx.bb
compile _release/samples/halo/Shadows/shadows.bb

compile _release/samples/mak/anim/anim.bb
compile _release/samples/mak/castle/castle.bb
compile _release/samples/mak/collide/collide.bb
compile _release/samples/mak/createanim/createanim.bb
compile _release/samples/mak/cubewater/cubewater.bb
compile _release/samples/mak/detailtex/detailtex.bb
compile _release/samples/mak/dragon/dragon.bb
compile _release/samples/mak/driver/driver.bb
compile _release/samples/mak/fakelight/fakelight.bb
compile _release/samples/mak/firepaint3d/firepaint3d.bb
compile _release/samples/mak/flag/flag.bb
compile _release/samples/mak/insaner/insaner.bb
compile _release/samples/mak/insectoids/insectoids.bb
compile _release/samples/mak/lights/lights.bb
compile _release/samples/mak/multi_tex/multi_tex.bb
compile _release/samples/mak/multicam/multicam.bb
compile _release/samples/mak/pick/pick.bb
compile _release/samples/mak/primitives/primitives.bb
compile _release/samples/mak/teapot/teapot.bb
compile _release/samples/mak/tron/tron.bb
compile _release/samples/mak/xfighter/xfighter.bb

compile _release/samples/si/fps/fps.bb
compile _release/samples/si/matrix/matrix.bb

compile _release/samples/Skully/flares/flares.bb

compile _release/samples/warpy/blitzdoc.bb

compile _release/samples/zenith/scare/scare.bb

echo "Run misc compiler flags"
check_flag -v
check_flag +k
check_flag -k
check_flag -l
check_flag -h

$BLITZCC -c -r test -a test/all.bb > /dev/null 2>&1
$BLITZCC -c -r test -c test/all.bb > /dev/null 2>&1
$BLITZCC -c -r test -j test/all.bb > /dev/null 2>&1

# test for syntax errors
$BLITZCC -c -r test -j test/syntax-errors/arithmetic-on-type.bb > /dev/null 2>&1
$BLITZCC -c -r test -j test/syntax-errors/division-by-zero.bb > /dev/null 2>&1

# invalid runtime
$BLITZCC -r d3d1 test/all.bb > /dev/null 2>&1

# generate the report...
ENV=$($BLITZCC -e)
if [ "$ENV" = "test" ]; then
  echo "Generating coverage report..."
  mkdir -p coverage
  gcovr -e '.*stdin.*' --html --html-details -o ./coverage/coverage.html build/
  cleanup
fi

exit $fail
