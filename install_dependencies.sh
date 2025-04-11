#!/usr/bin/env bash
#
# install_dependencies.sh
#
# Checks if essential packages for building modern OpenGL apps are installed.
# If not, installs them via apt on Debian/Ubuntu (including WSL).
#
# Usage: ./install_dependencies.sh
#

set -Eeuo pipefail

# ------------------------------------------------------------------------------
# 1) INSTALL DEPENDENCIES
# ------------------------------------------------------------------------------
# List of required packages for building/running a modern OpenGL + CMake project:
#   - build-essential: Provides gcc/g++, make, etc.
#   - cmake: For configuring/building projects
#   - libgl1-mesa-dev: Mesa OpenGL development libraries (includes libGL.so)
#   - xorg-dev: Provides GLX headers (GL/glx.h) and other X dev libraries
#
# Add more (e.g., libglu-dev, git) if your project needs them.
REQUIRED_PKGS=(
  build-essential
  cmake
  libgl1-mesa-dev
  xorg-dev
)

echo "Checking for required packages..."

# Collect any packages that are missing
MISSING=()

for pkg in "${REQUIRED_PKGS[@]}"; do
  if ! dpkg -s "$pkg" &>/dev/null; then
    MISSING+=("$pkg")
  fi
done

# If any are missing, install them
if [ ${#MISSING[@]} -eq 0 ]; then
  echo "All required packages are already installed."
else
  echo "The following packages are missing and will be installed: ${MISSING[*]}"
  sudo apt-get update
  sudo apt-get install -y "${MISSING[@]}"
fi

echo "Dependency installation check complete."
