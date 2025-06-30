# Installation Guide

This guide will help you install and build the AI Content Detector on different operating systems.

## Prerequisites

Before building the project, you need to install the following dependencies:

### Required Dependencies

1. **CMake** (version 3.16 or higher)
2. **C++ Compiler** with C++17 support
3. **OpenCV** (version 4.x)
4. **Eigen3** (version 3.3 or higher)

## Installation by Operating System

### Windows

#### Option 1: Using vcpkg (Recommended)

1. **Install vcpkg** (if not already installed):
   ```cmd
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
   ```

2. **Install dependencies**:
   ```cmd
   .\vcpkg install opencv4 eigen3
   ```

3. **Build the project**:
   ```cmd
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build . --config Release
   ```

#### Option 2: Manual Installation

1. **Install Visual Studio** with C++ development tools
2. **Download OpenCV** from https://opencv.org/releases/
3. **Download Eigen3** from https://eigen.tuxfamily.org/
4. **Set environment variables** for OpenCV and Eigen3 paths
5. **Build using CMake**:
   ```cmd
   mkdir build
   cd build
   cmake .. -G "Visual Studio 16 2019" -A x64
   cmake --build . --config Release
   ```

### macOS

#### Using Homebrew (Recommended)

1. **Install Homebrew** (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Install dependencies**:
   ```bash
   brew install cmake opencv eigen
   ```

3. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

#### Using MacPorts

1. **Install MacPorts** (if not already installed)
2. **Install dependencies**:
   ```bash
   sudo port install cmake opencv eigen3
   ```

3. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

### Linux (Ubuntu/Debian)

1. **Update package list**:
   ```bash
   sudo apt update
   ```

2. **Install dependencies**:
   ```bash
   sudo apt install build-essential cmake libopencv-dev libeigen3-dev
   ```

3. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

### Linux (CentOS/RHEL/Fedora)

1. **Install dependencies**:
   ```bash
   # CentOS/RHEL
   sudo yum install gcc-c++ cmake opencv-devel eigen3-devel
   
   # Fedora
   sudo dnf install gcc-c++ cmake opencv-devel eigen3-devel
   ```

2. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

### Linux (Arch Linux)

1. **Install dependencies**:
   ```bash
   sudo pacman -S base-devel cmake opencv eigen
   ```

2. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

## Verification

After building, verify the installation:

1. **Run the test script**:
   ```bash
   # Linux/macOS
   chmod +x test_build.sh
   ./test_build.sh
   
   # Windows
   test_build.bat
   ```

2. **Test the help command**:
   ```bash
   ./ai_detector help
   ```

## Troubleshooting

### Common Issues

#### CMake cannot find OpenCV

**Solution**: Set the OpenCV_DIR environment variable:
```bash
export OpenCV_DIR=/path/to/opencv/build
```

#### CMake cannot find Eigen3

**Solution**: Set the Eigen3_DIR environment variable:
```bash
export Eigen3_DIR=/path/to/eigen3
```

#### Compilation errors with C++17 features

**Solution**: Ensure your compiler supports C++17:
```bash
# Check GCC version (should be >= 7)
gcc --version

# Check Clang version (should be >= 5)
clang --version
```

#### OpenCV linking errors

**Solution**: Make sure OpenCV is properly installed and linked:
```bash
# Check if OpenCV is found
pkg-config --modversion opencv4
```

#### Memory allocation errors

**Solution**: Increase stack size or use dynamic allocation:
```bash
# Linux
ulimit -s 16384

# Windows: Use /STACK linker option
```

### Platform-Specific Issues

#### Windows

- **Visual Studio not found**: Install Visual Studio Build Tools
- **vcpkg path issues**: Use absolute paths in CMake configuration
- **DLL not found**: Add OpenCV bin directory to PATH

#### macOS

- **Xcode command line tools**: Install with `xcode-select --install`
- **Homebrew path issues**: Ensure Homebrew is in PATH
- **Permission errors**: Use `sudo` for system-wide installation

#### Linux

- **Package not found**: Update package repositories
- **Permission errors**: Use `sudo` for system-wide installation
- **Library path issues**: Set LD_LIBRARY_PATH environment variable

## Advanced Configuration

### Custom Build Options

You can customize the build with CMake options:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_CUDA=ON -DUSE_OPENMP=ON
```

### Available Options

- `CMAKE_BUILD_TYPE`: Debug, Release, RelWithDebInfo, MinSizeRel
- `USE_CUDA`: Enable CUDA acceleration (requires CUDA toolkit)
- `USE_OPENMP`: Enable OpenMP parallelization
- `BUILD_TESTS`: Build test suite
- `BUILD_EXAMPLES`: Build example programs

### Environment Variables

Set these environment variables for custom installations:

- `OpenCV_DIR`: Path to OpenCV installation
- `Eigen3_DIR`: Path to Eigen3 installation
- `CMAKE_PREFIX_PATH`: Additional search paths for packages

## Next Steps

After successful installation:

1. **Read the README.md** for usage instructions
2. **Try the examples** in the `examples/` directory
3. **Train your own model** with labeled data
4. **Customize the feature extraction** for your specific use case

## Support

If you encounter issues:

1. Check the troubleshooting section above
2. Verify all dependencies are correctly installed
3. Check the CMake output for specific error messages
4. Open an issue on the project repository with detailed error information 