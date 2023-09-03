#!/usr/bin/env bash

set -e # Fail on error

OUTDIR="out"

if [ -z "$1" ]; then
    echo "Usage: $0 <path/to/sources>"; exit
fi

if [ -d "$OUTDIR" ]; then
  echo Removing output directory "$OUTDIR".
  rm -rf "$OUTDIR"
fi

if ! [[ "$OSTYPE" =~ ^msys ]]; then
    ./build-mediapipe.sh "$1"
fi

./build-libfacedetection.sh "$1"
./build-libIFD.sh "$1"

./build-openpnp-capture.sh "$1"
./build-FaceViewer.sh "$1"
