#!/bin/bash

fail=0

RED='\033[0;31m'
GRN='\033[0;32m'
NC='\033[0m'

cleanup () {
  find . -name "*.gcda" -exec rm {} \;
  find . -name "*.gcov" -exec rm {} \;
}

result() {
  RESULT=$1
  if [ $RESULT -ne 0 ]
  then
    printf "  ${RED}FAIL${NC} $2\n"
    fail=1
  else
    printf "  ${GRN}PASS${NC} $2\n"
  fi
}

compile() {
  $BLITZCC -c "$1" > /dev/null 2>&1
  result $? "$1"
}

make_exe() {
  BASENAME="`basename $1 .bb`"
  DIR="`pwd`/$BASENAME"
  $BLITZCC -o $DIR "$1" > /dev/null 2>&1
  RESULT=$?
  result $RESULT "$1"

  if [ $RESULT -eq 0 ]
  then
    if [[ "$OSTYPE" == "darwin"* ]]
    then
      test -f $DIR.app/$BASENAME
      result $RESULT "  $BASENAME exists"
      test -f $DIR.app/Info.plist
      result $RESULT "  Info.plist exists"
      test -f $DIR.app/$BASENAME.icns
      result $RESULT "  $BASENAME.icns exists"
    fi
  fi
}

check_flag() {
  $BLITZCC $1 > /dev/null 2>&1
  result $? "$1"
}

BLITZCC=_release/bin/blitzcc

cleanup

# run the proper suite
$BLITZCC -r test test/all.bb
RESULT=$?
if [ $RESULT -ne 0 ]; then
  echo "Test suite failed. Fix it and then run the coverage generation again."
  fail=1
fi

echo "Verifying samples can compile"

compile _release/samples/AGore/BirdDemo/BirdDemo.bb
compile _release/samples/AGore/GrassDemo/Grass.bb
compile _release/samples/AGore/HeadDemo/TheHead.bb

compile _release/samples/birdie/2d-3dsprites/sprites.bb
compile "_release/samples/birdie/Brush Tiles/tt.bb"
compile _release/samples/birdie/CameraPickST/CameraPickST/main.bb
compile _release/samples/birdie/Explode/Explode.bb
compile "_release/samples/birdie/Fire Effect/Fire Effect/Main.bb"
compile "_release/samples/birdie/Jet Tails/Main.bb"
compile _release/samples/birdie/LodMesh/lmesh.bb
compile _release/samples/birdie/Mirror/mirror.bb
compile "_release/samples/birdie/Quick Deform/qd.bb"
compile "_release/samples/birdie/Spherical Landscapes/testbed.bb"
compile "_release/samples/birdie/Terrain Tiling/tt.bb"
compile _release/samples/birdie/UVMapping/UVMapping/test.bb
compile _release/samples/birdie/dolphin/dolphin.bb
compile _release/samples/birdie/dominos/dominos.bb
compile _release/samples/birdie/lodBalls/subdiv.bb
compile _release/samples/birdie/te/TE.bb
compile _release/samples/birdie/texpaint/main.bb
compile _release/samples/birdie/thunder/lightning.bb
compile _release/samples/birdie/thunder/thunder.bb

compile _release/samples/halo/Lightmap/lightmap.bb
compile _release/samples/halo/MeshFX/meshfx.bb
compile _release/samples/halo/Shadows/shadows.bb

compile "_release/samples/Hi-Toro/Death Island/deathisland.bb"
compile _release/samples/Hi-Toro/Shooter/Shooter/shooter-testbed.bb

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
compile _release/samples/mak/tex_render/tex_render.bb
compile _release/samples/mak/tron/tron.bb
compile _release/samples/mak/xfighter/xfighter.bb

compile "_release/samples/MattDavey/Matts Balls/bouncey.bb"

compile _release/samples/Richard_Betson/Big_Bang_1b/Big_Bang_1b.bb
compile _release/samples/Richard_Betson/Binary_Cage_12/Binary_Cage_12.bb
compile _release/samples/Richard_Betson/Power_fountain_b/power_fountian_b.bb
compile _release/samples/Richard_Betson/emerald_gate/Emerald_Gate_1.bb
compile _release/samples/Richard_Betson/orbit_nebula_source/orbit.bb

compile _release/samples/RobCummings/Bumpy/bumpyfun.bb
compile _release/samples/RobCummings/PyromaniaBB/PyromaniaBB-1.bb

compile _release/samples/RobHutchinson/BBLauncher/example.bb
compile _release/samples/RobHutchinson/BBLauncher/launcher2d.bb
compile _release/samples/RobHutchinson/BBLauncher/launcher3d.bb
compile _release/samples/RobHutchinson/BloxAndSpheres/blox\&spheres.bb
compile _release/samples/RobHutchinson/CraftFlare/CraftFlare.bb
compile _release/samples/RobHutchinson/CraftFlare/start.bb
compile _release/samples/RobHutchinson/ModelChildren/modelchildren.bb
compile _release/samples/RobHutchinson/ModelChildren/start.bb
compile _release/samples/RobHutchinson/SkyPlateau/SkyPlateau.bb
compile _release/samples/RobHutchinson/SkyPlateau/launcher3d.bb
compile _release/samples/RobHutchinson/WateryTerrain/start.bb
compile _release/samples/RobHutchinson/WateryTerrain/water.bb
compile _release/samples/RobHutchinson/WingPilot/WingPilotV0_01.bb

compile _release/samples/si/fps/fps.bb
compile _release/samples/si/matrix/matrix.bb

compile _release/samples/Skully/flares/flares.bb

compile _release/samples/warpy/blitzdoc.bb

compile _release/samples/zenith/scare/scare.bb

echo "Verify games compile"

compile _release/Games/bb3d_asteroids/EdzUpAsteroids.bb
compile _release/Games/wing_ring/wing_ring.bb
compile _release/Games/TunnelRun/tr.bb

echo "Generate executables"

make_exe _release/samples/mak/driver/driver.bundle.bb

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
