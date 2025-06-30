@echo off
echo ========================================
echo AI Content Detector - Project Test
echo ========================================
echo.

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo ❌ Error: CMakeLists.txt not found
    echo Please run this script from the project root directory.
    pause
    exit /b 1
)

echo ✅ Project structure found
echo.

REM Check for required files
echo Checking project files...
if exist "src\main.cpp" (
    echo ✅ Main source file found
) else (
    echo ❌ Main source file missing
)

if exist "include\ai_detector.h" (
    echo ✅ Main header file found
) else (
    echo ❌ Main header file missing
)

if exist "CMakeLists.txt" (
    echo ✅ CMake configuration found
) else (
    echo ❌ CMake configuration missing
)

echo.

REM Check for Visual Studio
echo Checking Visual Studio installation...
set "VS_FOUND="
if exist "C:\Program Files\Microsoft Visual Studio" (
    echo ✅ Visual Studio found in Program Files
    set "VS_FOUND=1"
)
if exist "C:\Program Files (x86)\Microsoft Visual Studio" (
    echo ✅ Visual Studio found in Program Files (x86)
    set "VS_FOUND=1"
)

if "%VS_FOUND%"=="" (
    echo ❌ Visual Studio not found
    echo Please install Visual Studio Community with C++ development tools
    goto :missing_deps
)

echo.

REM Check for C++ compiler
echo Checking C++ compiler...
where cl.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo ✅ C++ compiler (cl.exe) found in PATH
    set "COMPILER_FOUND=1"
) else (
    echo ❌ C++ compiler not found in PATH
    echo.
    echo This means you need to either:
    echo 1. Open "x64 Native Tools Command Prompt for VS 2022" (from Start Menu)
    echo 2. Or install the C++ development workload in Visual Studio
    echo.
    set "COMPILER_FOUND="
)

echo.

REM Check for CMake
echo Checking CMake...
where cmake >nul 2>&1
if %errorlevel% equ 0 (
    echo ✅ CMake found
    set "CMAKE_FOUND=1"
) else (
    echo ❌ CMake not found
    echo Please install CMake from https://cmake.org/download/
    set "CMAKE_FOUND="
)

echo.

REM Check for vcpkg
echo Checking vcpkg...
if exist "D:\vcpkg\vcpkg.exe" (
    echo ✅ vcpkg found
    set "VCPKG_FOUND=1"
) else (
    echo ❌ vcpkg not found
    echo Please install vcpkg and dependencies
    set "VCPKG_FOUND="
)

echo.

REM Summary
echo ========================================
echo Test Results Summary
echo ========================================

if "%VS_FOUND%"=="1" (
    echo ✅ Visual Studio: Installed
) else (
    echo ❌ Visual Studio: Missing
)

if "%COMPILER_FOUND%"=="1" (
    echo ✅ C++ Compiler: Available
) else (
    echo ❌ C++ Compiler: Not in PATH
)

if "%CMAKE_FOUND%"=="1" (
    echo ✅ CMake: Available
) else (
    echo ❌ CMake: Missing
)

if "%VCPKG_FOUND%"=="1" (
    echo ✅ vcpkg: Available
) else (
    echo ❌ vcpkg: Missing
)

echo.

REM Try to build if everything is available
if "%VS_FOUND%"=="1" if "%COMPILER_FOUND%"=="1" if "%CMAKE_FOUND%"=="1" if "%VCPKG_FOUND%"=="1" (
    echo 🚀 All dependencies found! Attempting to build...
    echo.
    goto :build_project
) else (
    echo ⚠️  Some dependencies are missing. See details above.
    goto :missing_deps
)

:build_project
echo Building the project...
if not exist "build" mkdir build
cd build

echo Configuring with CMake...
cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake
if errorlevel 1 (
    echo ❌ CMake configuration failed
    goto :build_failed
)

echo Building the project...
cmake --build . --config Release
if errorlevel 1 (
    echo ❌ Build failed
    goto :build_failed
)

if not exist "ai_detector.exe" (
    echo ❌ ai_detector.exe not found after build
    goto :build_failed
)

echo.
echo ========================================
echo 🎉 BUILD SUCCESSFUL!
echo ========================================
echo.
echo Testing the AI detector...
ai_detector.exe help
echo.
echo ========================================
echo Usage Examples:
echo ========================================
echo.
echo To detect AI content in an image:
echo   ai_detector.exe detect-image path\to\image.jpg
echo.
echo To detect AI content in a video:
echo   ai_detector.exe detect-video path\to\video.mp4
echo.
echo To train a custom model:
echo   ai_detector.exe train training_data\ output_model.bin
echo.
echo ========================================
echo Project is ready to use!
echo ========================================
goto :end

:build_failed
echo.
echo ========================================
echo ❌ Build Failed
echo ========================================
echo.
echo Please check the error messages above.
echo Common issues:
echo - Missing dependencies
echo - Incorrect vcpkg path
echo - Permission issues
echo.
echo Try running this script from "x64 Native Tools Command Prompt for VS 2022"
goto :end

:missing_deps
echo.
echo ========================================
echo 📋 Installation Guide
echo ========================================
echo.
echo To complete the setup:
echo.
echo 1. Install Visual Studio Community with "Desktop development with C++"
echo    Download: https://visualstudio.microsoft.com/vs/community/
echo.
echo 2. Install CMake
echo    Download: https://cmake.org/download/
echo.
echo 3. Install vcpkg and dependencies:
echo    git clone https://github.com/Microsoft/vcpkg.git D:\vcpkg
echo    cd D:\vcpkg
echo    .\bootstrap-vcpkg.bat
echo    .\vcpkg install opencv4 eigen3
echo.
echo 4. Open "x64 Native Tools Command Prompt for VS 2022"
echo 5. Navigate to this project: cd D:\task
echo 6. Run this script again: test_project.bat
echo.

:end
echo.
echo Press any key to exit...
pause >nul 