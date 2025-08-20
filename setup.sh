#!/usr/bin/env bash
set -e

echo "Installing dependencies..."
sudo pacman -S --needed cmake make gcc pkgconf glfw glew cairo mesa zip unzip tar curl ninja

echo "Cloning vcpkg..."
if [ ! -d "$HOME/vcpkg" ]; then
    git clone https://github.com/Microsoft/vcpkg.git "$HOME/vcpkg"
fi

echo "Bootstrapping vcpkg..."
cd "$HOME/vcpkg"
./bootstrap-vcpkg.sh

echo "Setting VCPKG_ROOT environment variable..."
if ! grep -q 'export VCPKG_ROOT=$HOME/vcpkg' "$HOME/.bashrc"; then
    echo 'export VCPKG_ROOT=$HOME/vcpkg' >> "$HOME/.bashrc"
fi
export VCPKG_ROOT="$HOME/vcpkg"
echo "VCPKG_ROOT set to $VCPKG_ROOT"

echo "Creating build directory..."
cd /home/frankfan/Projects/banim
mkdir -p build
cd build

echo "Configuring project with CMake..."
cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

echo "Building project..."
make

echo "Build complete. To run the project, go to the build directory and use:"
echo "  ./<project_binary_name>"