# Build Instructions - Enhanced Dungeon Crawler

This document provides detailed instructions for building the Enhanced Dungeon Crawler on different platforms.

## Prerequisites

### Required Software
- **C++17 compatible compiler**
  - GCC 7.0+ (Linux/macOS)
  - Clang 5.0+ (Linux/macOS)
  - MSVC 2017+ (Windows)
- **CMake 3.16 or higher**
- **SFML 2.5 or higher**

### Optional Tools
- **Git** (for version control)
- **Ninja** (faster builds)
- **Doxygen** (for documentation generation)

## Platform-Specific Setup

### Ubuntu/Debian Linux

#### Install Dependencies
```bash
# Update package list
sudo apt update

# Install build tools
sudo apt install build-essential cmake git

# Install SFML development libraries
sudo apt install libsfml-dev

# Optional: Install additional tools
sudo apt install ninja-build doxygen
```

#### Verify Installation
```bash
# Check compiler version
g++ --version

# Check CMake version
cmake --version

# Check SFML installation
pkg-config --modversion sfml-all
```

### CentOS/RHEL/Fedora

#### Install Dependencies
```bash
# For Fedora
sudo dnf install gcc-c++ cmake git sfml-devel

# For CentOS/RHEL (enable EPEL first)
sudo yum install epel-release
sudo yum install gcc-c++ cmake3 git
# SFML may need to be built from source on older versions
```

### macOS

#### Using Homebrew (Recommended)
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake sfml

# Optional tools
brew install ninja doxygen
```

#### Using MacPorts
```bash
# Install dependencies
sudo port install cmake sfml

# Optional tools
sudo port install ninja doxygen
```

### Windows

#### Using vcpkg (Recommended)
```bash
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat

# Install SFML
.\vcpkg install sfml:x64-windows

# Integrate with Visual Studio
.\vcpkg integrate install
```

#### Using MSYS2
```bash
# Update package database
pacman -Syu

# Install build tools
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-sfml

# Add to PATH
export PATH="/mingw64/bin:$PATH"
```

#### Manual Installation
1. Download and install Visual Studio 2019/2022 with C++ support
2. Download CMake from https://cmake.org/download/
3. Download SFML from https://www.sfml-dev.org/download.php
4. Extract SFML to a known location (e.g., `C:\SFML`)

## Building the Project

### Quick Start (All Platforms)
```bash
# Clone the repository
git clone <repository-url>
cd enhanced-dungeon-crawler

# Create build directory
mkdir build
cd build

# Configure the project
cmake ..

# Build the project
cmake --build . --config Release

# Run the game
./EnhancedDungeonCrawler  # Linux/macOS
# or
.\Release\EnhancedDungeonCrawler.exe  # Windows
```

### Detailed Build Process

#### 1. Clone and Navigate
```bash
git clone <repository-url>
cd enhanced-dungeon-crawler
```

#### 2. Create Build Directory
```bash
mkdir build
cd build
```

#### 3. Configure with CMake

##### Basic Configuration
```bash
cmake ..
```

##### Advanced Configuration Options
```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..

# Specify custom SFML location (if needed)
cmake -DSFML_ROOT=/path/to/sfml ..

# Use Ninja generator for faster builds
cmake -G Ninja ..

# Specify compiler explicitly
cmake -DCMAKE_CXX_COMPILER=g++-9 ..
```

##### Windows-Specific Configuration
```bash
# Using vcpkg
cmake -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ..

# Using Visual Studio generator
cmake -G "Visual Studio 16 2019" -A x64 ..

# Specify SFML location manually
cmake -DSFML_ROOT=C:/SFML ..
```

#### 4. Build the Project

##### Using CMake (Cross-platform)
```bash
# Build in Release mode
cmake --build . --config Release

# Build in Debug mode
cmake --build . --config Debug

# Build with multiple cores
cmake --build . --config Release --parallel 4
```

##### Using Make (Linux/macOS)
```bash
make -j4  # Build with 4 parallel jobs
```

##### Using Ninja
```bash
ninja
```

##### Using Visual Studio (Windows)
```bash
# Command line
msbuild EnhancedDungeonCrawler.sln /p:Configuration=Release

# Or open the .sln file in Visual Studio and build
```

## Troubleshooting

### Common Issues and Solutions

#### SFML Not Found
```bash
# Error: Could not find SFML
# Solution 1: Install SFML development packages
sudo apt install libsfml-dev  # Ubuntu/Debian

# Solution 2: Specify SFML location
cmake -DSFML_ROOT=/usr/local ..

# Solution 3: Set environment variable
export SFML_ROOT=/path/to/sfml
```

#### Compiler Version Issues
```bash
# Error: C++17 features not supported
# Solution: Use a newer compiler
sudo apt install gcc-9 g++-9  # Ubuntu
cmake -DCMAKE_CXX_COMPILER=g++-9 ..
```

#### Missing Assets
```bash
# Error: Cannot load textures/fonts
# Solution: Ensure assets directory is copied
cp -r ../assets .  # Linux/macOS
xcopy ..\assets . /E /I  # Windows
```

#### Windows-Specific Issues

##### MSVC Runtime Issues
```bash
# Install Visual C++ Redistributable
# Download from Microsoft website or use vcpkg
vcpkg install sfml:x64-windows
```

##### DLL Not Found
```bash
# Copy SFML DLLs to executable directory
# Or add SFML bin directory to PATH
set PATH=%PATH%;C:\SFML\bin
```

### Build Verification

#### Test the Build
```bash
# Run the executable
./EnhancedDungeonCrawler

# Check for missing dependencies (Linux)
ldd ./EnhancedDungeonCrawler

# Check for missing dependencies (macOS)
otool -L ./EnhancedDungeonCrawler

# Check for missing dependencies (Windows)
# Use Dependency Walker or similar tool
```

#### Performance Testing
```bash
# Run in debug mode for development
cmake --build . --config Debug
./EnhancedDungeonCrawler

# Run in release mode for performance
cmake --build . --config Release
./EnhancedDungeonCrawler
```

## Advanced Build Options

### Custom Build Configurations

#### Optimized Release Build
```bash
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG -march=native" \
      ..
```

#### Debug Build with Sanitizers
```bash
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS_DEBUG="-g -O0 -fsanitize=address -fsanitize=undefined" \
      ..
```

#### Profile-Guided Optimization
```bash
# Step 1: Build with profiling
cmake -DCMAKE_CXX_FLAGS="-fprofile-generate" ..
cmake --build .

# Step 2: Run the game to generate profile data
./EnhancedDungeonCrawler

# Step 3: Rebuild with profile data
cmake -DCMAKE_CXX_FLAGS="-fprofile-use" ..
cmake --build .
```

### Cross-Compilation

#### Linux to Windows (MinGW)
```bash
# Install MinGW cross-compiler
sudo apt install mingw-w64

# Configure for cross-compilation
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-w64.cmake ..
```

#### Static Linking
```bash
cmake -DCMAKE_CXX_FLAGS="-static-libgcc -static-libstdc++" ..
```

## Installation

### System Installation
```bash
# Install to system directories
sudo cmake --install . --config Release

# Install to custom directory
cmake --install . --prefix /opt/dungeon-crawler
```

### Package Creation
```bash
# Create DEB package (Ubuntu/Debian)
cpack -G DEB

# Create RPM package (CentOS/Fedora)
cpack -G RPM

# Create ZIP package (Windows)
cpack -G ZIP

# Create installer (Windows)
cpack -G NSIS
```

## Development Setup

### IDE Configuration

#### Visual Studio Code
1. Install C++ extension
2. Install CMake Tools extension
3. Open project folder
4. Configure CMake kit
5. Build and debug using integrated tools

#### CLion
1. Open project directory
2. CLion will automatically detect CMake
3. Configure toolchain if needed
4. Build and run using IDE controls

#### Visual Studio (Windows)
1. Open folder or CMakeLists.txt
2. Configure CMake settings
3. Build using Solution Explorer

### Continuous Integration

#### GitHub Actions Example
```yaml
name: Build
on: [push, pull_request]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Install dependencies
      run: sudo apt install libsfml-dev
    - name: Configure
      run: cmake -B build
    - name: Build
      run: cmake --build build
```

## Performance Optimization

### Compiler Optimizations
```bash
# Maximum optimization
cmake -DCMAKE_CXX_FLAGS="-O3 -DNDEBUG -march=native -flto" ..

# Size optimization
cmake -DCMAKE_CXX_FLAGS="-Os -DNDEBUG" ..
```

### Link-Time Optimization
```bash
cmake -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON ..
```

### Memory Optimization
```bash
# Use jemalloc for better memory allocation
sudo apt install libjemalloc-dev
cmake -DCMAKE_CXX_FLAGS="-ljemalloc" ..
```

## Documentation Generation

### Doxygen Documentation
```bash
# Generate documentation (if Doxygen is available)
cmake --build . --target doc_doxygen

# View documentation
open docs/html/index.html  # macOS
xdg-open docs/html/index.html  # Linux
start docs/html/index.html  # Windows
```

---

For additional help or issues not covered here, please refer to the main README.md or create an issue in the project repository.

