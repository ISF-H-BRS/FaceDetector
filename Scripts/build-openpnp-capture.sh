#!/usr/bin/env bash

set -e # Fail on error

SNAPSHOT="0.0.28"

BUILDDIR="build-openpnp-capture"
OUTDIR="out"

if [ -z "$1" ]; then
    echo "Usage: $0 <path/to/sources>"; exit
fi

SRCDIR="$1/3rdparty/openpnp-capture-$SNAPSHOT"

if [ -d "$BUILDDIR" ]; then
  echo Removing directory "$BUILDDIR."
  rm -rf "$BUILDDIR"
fi

mkdir "$BUILDDIR"
mkdir -p "$OUTDIR"

CMAKE_ARGS=(-DCMAKE_BUILD_TYPE=Release)

if [[ "$OSTYPE" =~ ^msys ]]; then
    CMAKE_ARGS=(${CMAKE_ARGS[@]} -G "MSYS Makefiles")
    LIBRARY_NAME="libopenpnp-capture.dll"
else
    LIBRARY_NAME="libopenpnp-capture.so"
fi

cmake -B "$BUILDDIR" "${CMAKE_ARGS[@]}" "$SRCDIR"
cmake --build "$BUILDDIR" -- -j$(nproc)

cp "$SRCDIR/include/openpnp-capture.h" ./
cp "$BUILDDIR/$LIBRARY_NAME" "$OUTDIR/"
