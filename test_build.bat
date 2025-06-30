@echo off
echo AI Content Detector - Build Test
echo ================================

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo Error: CMakeLists.txt not found. Please run this script from the project root.
    exit /b 1
)

REM Create build directory
echo Creating build directory...
if not exist "build" mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake .. >nul 2>&1
if errorlevel 1 (
    echo Error: CMake configuration failed.
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . >nul 2>&1
if errorlevel 1 (
    echo Error: Build failed.
    exit /b 1
)

REM Check if executable was created
if not exist "ai_detector.exe" (
    echo Error: Executable 'ai_detector.exe' not found after build.
    exit /b 1
)

echo Build successful! Executable created: ai_detector.exe

REM Test basic functionality
echo.
echo Testing basic functionality...

REM Test help command
echo Testing help command...
ai_detector.exe help >nul 2>&1
if errorlevel 1 (
    echo Error: Help command failed.
    exit /b 1
)

echo Help command works correctly.

REM Test with invalid command
echo Testing invalid command handling...
ai_detector.exe invalid_command >nul 2>&1
if not errorlevel 1 (
    echo Error: Invalid command should have failed.
    exit /b 1
)

echo Invalid command handling works correctly.

REM Test with missing arguments
echo Testing missing arguments handling...
ai_detector.exe detect-image >nul 2>&1
if not errorlevel 1 (
    echo Error: Missing arguments should have failed.
    exit /b 1
)

echo Missing arguments handling works correctly.

echo.
echo All tests passed! The AI detector is ready to use.
echo.
echo Usage examples:
echo   ai_detector.exe help
echo   ai_detector.exe detect-image ^<image_path^>
echo   ai_detector.exe detect-video ^<video_path^>
echo   ai_detector.exe train ^<training_data_path^> ^<output_model_path^>

pause 