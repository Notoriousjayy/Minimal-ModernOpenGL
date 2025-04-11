#!/usr/bin/env bash
#
# build_and_run.sh
#
# Builds the CMake project in a "build" folder, then runs the resulting executable.
# Assumes dependencies are already installed (e.g., via ./install_dependencies.sh).
#
# Usage: ./build_and_run.sh
#

set -Eeuo pipefail

# ------------------------------------------------------------------------------
# 1) BUILD THE CMAKE PROJECT
# ------------------------------------------------------------------------------
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

BUILD_DIR="build"
EXECUTABLE_NAME="ModernGLProject"  # <-- Change if your exe name differs

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Configuring project with CMake..."
cmake ..

echo "Building project..."
make -j"$(nproc)"  # Parallel build for speed; remove -j if you prefer serial

# ------------------------------------------------------------------------------
# 2) RUN THE PROGRAM
# ------------------------------------------------------------------------------
if [ -f "./$EXECUTABLE_NAME" ]; then
  echo "Running $EXECUTABLE_NAME..."
  "./$EXECUTABLE_NAME"
else
  echo "Error: $EXECUTABLE_NAME not found in build folder."
  exit 1
fi
