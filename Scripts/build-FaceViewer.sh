#!/usr/bin/env bash

set -e # Fail on error

BUILDDIR="build-FaceViewer"
OUTDIR="out"

if [ -z "$1" ]; then
    echo "Usage: $0 <path/to/sources>"; exit
fi

SRCDIR="$1/FaceViewer"

if [ -d "$BUILDDIR" ]; then
  echo Removing directory "$BUILDDIR".
  rm -rf "$BUILDDIR"
fi

mkdir "$BUILDDIR"
mkdir -p "$OUTDIR"

CMAKE_ARGS=(-DCMAKE_BUILD_TYPE=Release)

if [[ "$OSTYPE" =~ ^msys ]]; then
    CMAKE_ARGS=(${CMAKE_ARGS[@]} -G "MSYS Makefiles")
    PROGRAM_NAME="FaceViewer.exe"
else
    PROGRAM_NAME="FaceViewer"
fi

cmake -B "$BUILDDIR" "${CMAKE_ARGS[@]}" -DCMAKE_CXX_FLAGS="-L../$OUTDIR -I.." "$SRCDIR"
cmake --build "$BUILDDIR" -- -j$(nproc)

cp -f "$BUILDDIR/$PROGRAM_NAME" "$OUTDIR/"
