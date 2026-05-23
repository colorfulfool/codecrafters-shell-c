#!/bin/sh
set -e

# ================================================
# Compile
# ================================================
(
  cd "$(dirname "$0")"

  # Set VCPKG_ROOT if not already set
  if [ -z "${VCPKG_ROOT}" ]; then
    if [ -d "$HOME/vcpkg" ]; then
      export VCPKG_ROOT="$HOME/vcpkg"
    elif [ -d "/usr/local/vcpkg" ]; then
      export VCPKG_ROOT="/usr/local/vcpkg"
    else
      echo "Error: VCPKG_ROOT is not set and vcpkg not found in default locations."
      echo "Please run: export VCPKG_ROOT=~/vcpkg   (or wherever you installed it)"
      exit 1
    fi
  fi

  echo "Using VCPKG_ROOT: $VCPKG_ROOT"

  cmake -B build -S . \
    -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" \
    -DCMAKE_BUILD_TYPE=Release

  cmake --build ./build --config Release
)

# ================================================
# Run
# ================================================
exec "$(dirname "$0")/build/shell" "$@"
