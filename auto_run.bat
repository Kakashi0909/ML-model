@echo off
setlocal enabledelayedexpansion

echo ========================================
echo AI Content Detector - Automated Build and Run
echo ========================================
echo.

REM Set colors for better output
set "GREEN=[92m"
set "RED=[91m"
set "YELLOW=[93m"
set "BLUE=[94m"
set "RESET=[0m"

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo %RED%Error: CMakeLists.txt not found. Please run this script from the project root.%RESET%
    pause
    exit /b 1
)

echo %BLUE%Step 1: Checking prerequisites...%RESET%

REM Check for CMake
cmake --version >nul 2>&1
if errorlevel 1 (
    echo %RED%Error: CMake not found!%RESET%
    echo Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)
echo %GREEN%✓ CMake found%RESET%

REM Check for Visual Studio compiler
cl >nul 2>&1
if errorlevel 1 (
    echo %YELLOW%Warning: Visual Studio compiler not found in PATH%RESET%
    echo Attempting to find Visual Studio...
    
    REM Try to find Visual Studio
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
        echo %GREEN%Found Visual Studio 2022 Community%RESET%
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
        echo %GREEN%Found Visual Studio 2022 Professional%RESET%
        call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
        echo %GREEN%Found Visual Studio 2022 Enterprise%RESET%
        call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    ) else (
        echo %RED%Error: Visual Studio not found!%RESET%
        echo Please install Visual Studio with C++ development tools
        pause
        exit /b 1
    )
) else (
    echo %GREEN%✓ Visual Studio compiler found%RESET%
)

echo.
echo %BLUE%Step 2: Creating build directory...%RESET%
if not exist "build" mkdir build
cd build

echo.
echo %BLUE%Step 3: Configuring with CMake...%RESET%

REM Try different CMake configurations
set "CMAKE_SUCCESS=0"

REM Try with vcpkg if available
if exist "D:\vcpkg\scripts\buildsystems\vcpkg.cmake" (
    echo %YELLOW%Attempting to configure with vcpkg...%RESET%
    cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake
    if not errorlevel 1 (
        set "CMAKE_SUCCESS=1"
        echo %GREEN%✓ Configured with vcpkg%RESET%
    )
)

REM If vcpkg failed, try without it
if "!CMAKE_SUCCESS!"=="0" (
    echo %YELLOW%Attempting to configure without vcpkg...%RESET%
    cmake ..
    if not errorlevel 1 (
        set "CMAKE_SUCCESS=1"
        echo %GREEN%✓ Configured successfully%RESET%
    )
)

REM If still failed, try with specific generator
if "!CMAKE_SUCCESS!"=="0" (
    echo %YELLOW%Attempting to configure with Visual Studio generator...%RESET%
    cmake .. -G "Visual Studio 17 2022"
    if not errorlevel 1 (
        set "CMAKE_SUCCESS=1"
        echo %GREEN%✓ Configured with Visual Studio generator%RESET%
    )
)

if "!CMAKE_SUCCESS!"=="0" (
    echo.
    echo %RED%CMake configuration failed!%RESET%
    echo.
    echo Please make sure you have installed:
    echo - Visual Studio with C++ development tools
    echo - CMake
    echo - OpenCV and Eigen3 (via vcpkg or manual installation)
    echo.
    echo Installation instructions:
    echo 1. Install Visual Studio Community with "Desktop development with C++"
    echo 2. Install CMake from https://cmake.org/download/
    echo 3. For dependencies, either:
    echo    a) Use vcpkg: git clone https://github.com/Microsoft/vcpkg.git
    echo       cd vcpkg ^&^& .\bootstrap-vcpkg.bat
    echo       .\vcpkg install opencv4 eigen3
    echo    b) Or install manually from their respective websites
    echo.
    pause
    exit /b 1
)

echo.
echo %BLUE%Step 4: Building the project...%RESET%
cmake --build . --config Release
if errorlevel 1 (
    echo.
    echo %RED%Build failed!%RESET%
    echo Please check the error messages above.
    pause
    exit /b 1
)

echo.
echo %BLUE%Step 5: Testing the build...%RESET%
if not exist "ai_detector.exe" (
    echo %RED%Error: ai_detector.exe not found after build.%RESET%
    echo Checking for alternative executable names...
    dir *.exe
    pause
    exit /b 1
)

echo %GREEN%✓ Build successful! AI Detector is ready.%RESET%

echo.
echo ========================================
echo %GREEN%BUILD SUCCESSFUL!%RESET%
echo ========================================
echo.

echo %BLUE%Testing the help command:%RESET%
ai_detector.exe help

echo.
echo ========================================
echo %YELLOW%USAGE EXAMPLES:%RESET%
echo ========================================
echo.
echo %GREEN%To detect AI content in an image:%RESET%
echo   ai_detector.exe detect-image path\to\your\image.jpg
echo.
echo %GREEN%To detect AI content in a video:%RESET%
echo   ai_detector.exe detect-video path\to\your\video.mp4
echo.
echo %GREEN%To train a custom model:%RESET%
echo   ai_detector.exe train path\to\training\data\ output_model.bin
echo.
echo %GREEN%To use a specific model:%RESET%
echo   ai_detector.exe detect-image image.jpg my_model.bin
echo.

echo %BLUE%Would you like to run a demo test? (y/n)%RESET%
set /p "run_demo="
if /i "!run_demo!"=="y" (
    echo.
    echo %BLUE%Running demo test...%RESET%
    echo %YELLOW%Note: This will create a test image if none exists%RESET%
    
    REM Check if demo.cpp exists and compile it
    if exist "..\demo.cpp" (
        echo %BLUE%Compiling demo program...%RESET%
        cl /EHsc ..\demo.cpp /Fe:demo.exe
        if not errorlevel 1 (
            echo %GREEN%✓ Demo compiled successfully%RESET%
            echo %BLUE%Running demo...%RESET%
            demo.exe
        ) else (
            echo %RED%Demo compilation failed%RESET%
        )
    ) else (
        echo %YELLOW%Demo file not found, skipping demo test%RESET%
    )
)

echo.
echo %GREEN%Project is ready to use!%RESET%
echo %BLUE%Press any key to exit...%RESET%
pause >nul 