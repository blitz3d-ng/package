#!/bin/sh

##
# A basic docker wrapper for our environment containers.
# See https://github.com/blitz3d-ng/env

export PLATFORM=$1
shift

IMAGE=ghcr.io/blitz3d-ng/env:$PLATFORM
OPTIONS="--rm -w /b3d -v $(pwd):/b3d"

if [[ -t 1 ]]
then
  OPTIONS="-it $OPTIONS"
fi

docker run $OPTIONS $IMAGE $@
