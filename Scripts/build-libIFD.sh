#!/usr/bin/env bash

set -e # Fail on error

BACKEND_ARGS="-DIFD_USE_LIBFACEDETECTION=1 -DIFD_USE_DLIB=1 -DIFD_USE_OPENCV=1"

if [[ "$OSTYPE" =~ ^msys ]]; then
    BACKEND_ARGS="$BACKEND_ARGS -DIFD_USE_MEDIAPIPE=0"
else
    BACKEND_ARGS="$BACKEND_ARGS -DIFD_USE_MEDIAPIPE=1"
fi

BUILDDIR="build-libIFD"
OUTDIR="out"

if [ -z "$1" ]; then
    echo "Usage: $0 <path/to/sources>"; exit
fi

SRCDIR="$1/libIFD"

if [ -d "$BUILDDIR" ]; then
  echo Removing directory "$BUILDDIR."
  rm -rf "$BUILDDIR"
fi

mkdir "$BUILDDIR"
mkdir -p "$OUTDIR"

CMAKE_ARGS=(-DCMAKE_BUILD_TYPE=Release $BACKEND_ARGS)

if [[ "$OSTYPE" =~ ^msys ]]; then
    CMAKE_ARGS=(${CMAKE_ARGS[@]} -G "MSYS Makefiles")
    LIBRARY_NAME="IFD.dll"
else
    LIBRARY_NAME="libIFD.so"
fi

cmake -B "$BUILDDIR" "${CMAKE_ARGS[@]}" -DCMAKE_CXX_FLAGS="-L../$OUTDIR -I.." "$SRCDIR"
cmake --build "$BUILDDIR" -- -j$(nproc)

cp -f "$BUILDDIR/$LIBRARY_NAME" "$SRCDIR/include/ifd.h" "$OUTDIR/"
