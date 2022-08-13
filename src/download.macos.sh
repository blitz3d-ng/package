#!/bin/sh

##
# Blitz3D "NG" Downloader
#
# This script will download a particular build of release from GitHub and then
# ad-hoc sign the executables so that they can run without issues from Gatekeeper.
#
set -e

ARCHIVE=blitz3d-ng-$TAG-$MACOS.zip

echo Downloading $ARCHIVE...
curl -o b3d-$TAG.zip -L https://github.com/blitz3d-ng/blitz3d-ng/releases/download/$TAG/blitz3d-ng-$TAG-$MACOS.zip

mkdir -p b3d-$TAG
cd b3d-$TAG

unzip ../b3d-$TAG.zip

for file in Blitz3D.app/Contents/MacOS/Blitz3D bin/blitzcc bin/ide2
do
  echo Signing $file...
  chmod +x $file
  codesign -s - $file
done

echo Success!
