#!/usr/bin/env bash

set -e # Fail on error

SNAPSHOT="20220728"

BUILDDIR="build-libfacedetection"
OUTDIR="out"

if [ -z "$1" ]; then
    echo "Usage: $0 <path/to/sources>"; exit
fi

SRCDIR="$1/3rdparty/libfacedetection-$SNAPSHOT"

if [ -d "$BUILDDIR" ]; then
  echo Removing directory "$BUILDDIR."
  rm -rf "$BUILDDIR"
fi

mkdir "$BUILDDIR"
mkdir -p "$OUTDIR"

CMAKE_ARGS=(-DCMAKE_BUILD_TYPE=Release -DUSE_OPENMP=0 -DBUILD_SHARED_LIBS=1)

if [[ "$OSTYPE" =~ ^msys ]]; then
    CMAKE_ARGS=(${CMAKE_ARGS[@]} -G "MSYS Makefiles")
    LIBRARY_NAME="libfacedetection.dll"
else
    LIBRARY_NAME="libfacedetection.so"
fi

cmake -B "$BUILDDIR" "${CMAKE_ARGS[@]}" "$SRCDIR"
cmake --build "$BUILDDIR" -- -j$(nproc)

cp -f "$BUILDDIR/facedetection_export.h" "$SRCDIR/src/facedetectcnn.h" ./
cp -f "$BUILDDIR/$LIBRARY_NAME" "$OUTDIR/"
