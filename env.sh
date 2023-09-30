#!/bin/sh

set -e

##
# A basic docker wrapper for our environment containers.
# See https://github.com/blitz3d-ng/env

export PLATFORM=$1
shift

VOLUME=blitz3d-ng-gems-$PLATFORM
IMAGE=ghcr.io/blitz3d-ng/env:$PLATFORM
OPTIONS="--cap-add=SYS_PTRACE --security-opt seccomp=unconfined --rm -w /b3d -v $(pwd):/b3d -v $VOLUME:/bundle -e LLVM_ROOT=/opt/llvm -e blitzpath=/b3d/_release -e BUNDLE_PATH=/bundle"

if [ -f /dev/snd ]
then
  OPTIONS="--device /dev/snd $OPTIONS"
fi

if [ -t 1 ]
then
  OPTIONS="--rm -it $OPTIONS"
fi

docker run $OPTIONS $IMAGE $@
