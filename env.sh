#!/bin/sh

##
# A basic script which either configures the current shell
# or starts docker.
#
# See https://github.com/blitz3d-ng/env for info on docker.

# if no params, export needed variables for the compiler.
if [ "$1" = "" ]
then
    dir=$(cd "$(dirname "$0")"; pwd -P)
    export blitzpath=$dir/_release
    export PATH=$blitzpath/bin:$PATH
    return 0
fi

# otherwise, try docker
set -e

export PLATFORM=$1
shift

VERSION="${VERSION:-v10}"

VOLUME=blitz3d-ng-gems-$PLATFORM
IMAGE=ghcr.io/blitz3d-ng/env:$PLATFORM-$VERSION
OPTIONS="--cap-add=SYS_PTRACE --security-opt seccomp=unconfined --rm -w /b3d -v $(pwd):/b3d -v $VOLUME:/bundle -e LLVM_ROOT=/opt/llvm -e blitzpath=/b3d/_release -e BUNDLE_PATH=/bundle"

if [ -d /dev/snd ]
then
  OPTIONS="--device /dev/snd $OPTIONS"
fi

if [ -t 1 ]
then
  OPTIONS="--rm -it $OPTIONS"
fi

docker run $OPTIONS $IMAGE $@
