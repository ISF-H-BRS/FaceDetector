#!/usr/bin/env bash

set -e # Fail on error

if [[ "$OSTYPE" =~ ^msys ]]; then
    echo "Building MediaPipe with MinGW on MSYS2 is currently not supported."
    echo "See https://google.github.io/mediapipe/getting_started/install.html#installing-on-windows "
    echo "and https://bazel.build/install/windows for details."
    exit
fi

WORKDIR="$(pwd)"

BUILDDIR="build-mediapipe"
OUTDIR="out"

if [ -z "$1" ]; then
    echo "Usage: $0 <path/to/sources>"; exit
fi

SRCDIR="$1/libIFD"
MPDIR="$SRCDIR/mediapipe"

if [ -d "$BUILDDIR" ]; then
  echo Removing directory "$BUILDDIR."
  rm -rf "$BUILDDIR"
fi

mkdir "$BUILDDIR"
mkdir -p "$OUTDIR"

BAZELDIR="$BUILDDIR/IFD"
BINDIR="$BUILDDIR/bazel-bin/IFD"

git clone https://github.com/google/mediapipe.git "$BUILDDIR"
mkdir "$BAZELDIR"

cp "$MPDIR/BUILD" "$MPDIR/mediapipebackendimpl.cpp" "$MPDIR/mediapipebackendimpl.h" "$BAZELDIR/"
cp "$SRCDIR/include/ifd.h" "$BAZELDIR/"

cd "$BUILDDIR"
git checkout dea6ccba258f8ae58f2af4a9b1be749cbc1f91b7
sed -i 's@#"include/opencv4/@"include/opencv4/@g' "third_party/opencv_linux.BUILD"

bazel build --compilation_mode opt --define MEDIAPIPE_DISABLE_OPENCV=1 IFD:mediapipe

cd "$WORKDIR"
cp -f "$BINDIR/libmediapipe.so" "$OUTDIR/"
cp -R "$MPDIR/mediapipe" "$OUTDIR/"
