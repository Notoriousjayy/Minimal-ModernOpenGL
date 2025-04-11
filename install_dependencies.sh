#!/usr/bin/env bash
#
# build_and_run.sh
#
# 1) Checks if essential packages for building modern OpenGL apps are installed.
#    If not, installs them via apt on Debian/Ubuntu (including WSL).
# 2) Builds the CMake project in a "build" folder.
# 3) Runs the resulting executable.

set -Eeuo pipefail

# ------------------------------
# 1) INSTALL DEPENDENCIES
# ------------------------------
# List of required packages for building/running a modern OpenGL + CMake project:
#   - build-essential: Provides gcc/g++, make, etc.
#   - cmake: For configuring/building projects.
#   - libgl1-mesa-dev: Mesa OpenGL development libraries (includes libGL.so).
#   - xorg-dev: Provides GLX headers (GL/glx.h) and other X dev libraries.
#
# Add more (e.g., libglu-dev, git) if your project needs them.
REQUIRED_PKGS=(
  build-essential
  cmake
  libgl1-mesa-dev
  xorg-dev
)

echo "Checking for required packages..."

MISSING=()

for pkg in "${REQUIRED_PKGS[@]}"; do
  if ! dpkg -s "$pkg" &>/dev/null; then
    MISSING+=("$pkg")
  fi
done

if [ ${#MISSING[@]} -eq 0 ]; then
  echo "All required packages are already installed."
else
  echo "The following packages are missing and will be installed: ${MISSING[*]}"
  sudo apt-get update
  sudo apt-get install -y "${MISSING[@]}"
fi

echo "Dependency installation check complete."

# ------------------------------
# 2) BUILD THE CMAKE PROJECT
# ------------------------------
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

BUILD_DIR="build"
EXECUTABLE_NAME="ModernGLProject"  # <-- Change if your exe name differs

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Configuring project with CMake..."
cmake ..

echo "Building project..."
make -j$(nproc)  # Parallel build for speed; remove -j if you prefer serial

# ------------------------------
# 3) RUN THE PROGRAM
# ------------------------------
if [ -f "./$EXECUTABLE_NAME" ]; then
  echo "Running $EXECUTABLE_NAME..."
  "./$EXECUTABLE_NAME"
else
  echo "Error: $EXECUTABLE_NAME not found in build folder."
  exit 1
fi
