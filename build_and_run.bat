@echo off
echo ========================================
echo AI Content Detector - Build and Run
echo ========================================
echo.

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo Error: CMakeLists.txt not found. Please run this script from the project root.
    pause
    exit /b 1
)

echo Step 1: Creating build directory...
if not exist "build" mkdir build
cd build

echo Step 2: Configuring with CMake...
echo Note: This requires CMake and Visual Studio to be installed.
cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake
if errorlevel 1 (
    echo.
    echo CMake configuration failed!
    echo Please make sure you have installed:
    echo - Visual Studio with C++ development tools
    echo - CMake
    echo - vcpkg with OpenCV and Eigen3
    echo.
    echo Installation instructions:
    echo 1. Install Visual Studio Community with "Desktop development with C++"
    echo 2. Install CMake from https://cmake.org/download/
    echo 3. Run: git clone https://github.com/Microsoft/vcpkg.git
    echo 4. Run: cd vcpkg && .\bootstrap-vcpkg.bat
    echo 5. Run: .\vcpkg install opencv4 eigen3
    echo.
    pause
    exit /b 1
)

echo Step 3: Building the project...
cmake --build . --config Release
if errorlevel 1 (
    echo.
    echo Build failed!
    echo Please check the error messages above.
    pause
    exit /b 1
)

echo Step 4: Testing the build...
if not exist "ai_detector.exe" (
    echo Error: ai_detector.exe not found after build.
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build successful! AI Detector is ready.
echo ========================================
echo.

echo Testing the help command:
ai_detector.exe help

echo.
echo ========================================
echo Usage Examples:
echo ========================================
echo.
echo To detect AI content in an image:
echo   ai_detector.exe detect-image path\to\your\image.jpg
echo.
echo To detect AI content in a video:
echo   ai_detector.exe detect-video path\to\your\video.mp4
echo.
echo To train a custom model:
echo   ai_detector.exe train path\to\training\data\ output_model.bin
echo.
echo To use a specific model:
echo   ai_detector.exe detect-image image.jpg my_model.bin
echo.

pause 