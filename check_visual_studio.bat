@echo off
echo ========================================
echo Visual Studio Installation Checker
echo ========================================
echo.

echo Checking for Visual Studio installations...
echo.

REM Check for Visual Studio installations
set "VS_FOUND="
set "VS_VERSION="

REM Check Program Files
if exist "C:\Program Files\Microsoft Visual Studio" (
    echo Found Visual Studio in Program Files
    set "VS_FOUND=1"
    for /d %%i in ("C:\Program Files\Microsoft Visual Studio\*") do (
        if "%%~ni"=="2022" (
            echo   - Visual Studio 2022 found
            set "VS_VERSION=2022"
        ) else if "%%~ni"=="2019" (
            echo   - Visual Studio 2019 found
            set "VS_VERSION=2019"
        ) else if "%%~ni"=="2017" (
            echo   - Visual Studio 2017 found
            set "VS_VERSION=2017"
        )
    )
)

REM Check Program Files (x86)
if exist "C:\Program Files (x86)\Microsoft Visual Studio" (
    echo Found Visual Studio in Program Files (x86)
    set "VS_FOUND=1"
    for /d %%i in ("C:\Program Files (x86)\Microsoft Visual Studio\*") do (
        if "%%~ni"=="2022" (
            echo   - Visual Studio 2022 found
            set "VS_VERSION=2022"
        ) else if "%%~ni"=="2019" (
            echo   - Visual Studio 2019 found
            set "VS_VERSION=2019"
        ) else if "%%~ni"=="2017" (
            echo   - Visual Studio 2017 found
            set "VS_VERSION=2017"
        )
    )
)

echo.

if "%VS_FOUND%"=="1" (
    echo ✅ Visual Studio is installed!
    echo.
    echo Checking for C++ development tools...
    echo.
    
    REM Check for MSVC compiler
    where cl.exe >nul 2>&1
    if %errorlevel% equ 0 (
        echo ✅ C++ compiler (cl.exe) found
    ) else (
        echo ❌ C++ compiler (cl.exe) not found
        echo.
        echo This means the "Desktop development with C++" workload
        echo was not installed or is not in the PATH.
        echo.
        echo Please:
        echo 1. Open Visual Studio Installer
        echo 2. Modify your Visual Studio installation
        echo 3. Select "Desktop development with C++" workload
        echo 4. Install the missing components
    )
    
    echo.
    echo Checking for Native Tools Command Prompt...
    echo.
    
    REM Check if we can find the command prompt
    if exist "%ProgramData%\Microsoft\Windows\Start Menu\Programs\Visual Studio %VS_VERSION%\Visual Studio Tools\VC\*Native*" (
        echo ✅ Native Tools Command Prompt should be available
        echo.
        echo You can find it in Start Menu by searching for:
        echo "x64 Native Tools Command Prompt for VS %VS_VERSION%"
    ) else (
        echo ❌ Native Tools Command Prompt not found
        echo.
        echo This suggests the C++ development tools are not properly installed.
    )
    
) else (
    echo ❌ Visual Studio not found!
    echo.
    echo Please install Visual Studio Community with C++ development tools:
    echo 1. Download from: https://visualstudio.microsoft.com/vs/community/
    echo 2. During installation, select "Desktop development with C++"
    echo 3. Run this script again after installation
)

echo.
echo ========================================
echo Check Complete
echo ========================================
echo.

if "%VS_FOUND%"=="1" (
    echo If all checks passed, you can now:
    echo 1. Open "x64 Native Tools Command Prompt for VS %VS_VERSION%"
    echo 2. Navigate to your project: cd D:\task
    echo 3. Run: build_and_run.bat
) else (
    echo Please install Visual Studio first, then run this script again.
)

echo.
pause 