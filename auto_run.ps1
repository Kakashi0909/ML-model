# AI Content Detector - Automated Build and Run Script
# PowerShell Version

param(
    [switch]$SkipDemo,
    [switch]$Verbose,
    [string]$BuildType = "Release"
)

# Set console colors
$Host.UI.RawUI.ForegroundColor = "White"
Clear-Host

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "AI Content Detector - Automated Build and Run" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Function to write colored output
function Write-Status {
    param(
        [string]$Message,
        [string]$Status = "Info"
    )
    
    switch ($Status) {
        "Success" { Write-Host "✓ $Message" -ForegroundColor Green }
        "Error" { Write-Host "✗ $Message" -ForegroundColor Red }
        "Warning" { Write-Host "⚠ $Message" -ForegroundColor Yellow }
        "Info" { Write-Host "→ $Message" -ForegroundColor Blue }
        default { Write-Host $Message }
    }
}

# Function to check if command exists
function Test-Command {
    param([string]$Command)
    try {
        Get-Command $Command -ErrorAction Stop | Out-Null
        return $true
    }
    catch {
        return $false
    }
}

# Check if we're in the right directory
if (-not (Test-Path "CMakeLists.txt")) {
    Write-Status "CMakeLists.txt not found. Please run this script from the project root." "Error"
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Status "Step 1: Checking prerequisites..." "Info"

# Check for CMake
if (-not (Test-Command "cmake")) {
    Write-Status "CMake not found!" "Error"
    Write-Host "Please install CMake from https://cmake.org/download/" -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit 1
}
Write-Status "CMake found" "Success"

# Check for Visual Studio compiler
$vsFound = $false
if (Test-Command "cl") {
    Write-Status "Visual Studio compiler found" "Success"
    $vsFound = $true
} else {
    Write-Status "Visual Studio compiler not found in PATH" "Warning"
    Write-Host "Attempting to find Visual Studio..." -ForegroundColor Yellow
    
    # Try to find Visual Studio and set up environment
    $vsPaths = @(
        "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat",
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat",
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    )
    
    foreach ($path in $vsPaths) {
        if (Test-Path $path) {
            Write-Status "Found Visual Studio at: $path" "Success"
            Write-Host "Setting up Visual Studio environment..." -ForegroundColor Yellow
            
            # Execute the vcvars batch file and capture environment
            $vcvarsOutput = cmd /c "`"$path`" && set" 2>$null
            if ($vcvarsOutput) {
                foreach ($line in $vcvarsOutput) {
                    if ($line -match "^([^=]+)=(.*)$") {
                        $name = $matches[1]
                        $value = $matches[2]
                        [Environment]::SetEnvironmentVariable($name, $value, "Process")
                    }
                }
                $vsFound = $true
                break
            }
        }
    }
    
    if (-not $vsFound) {
        Write-Status "Visual Studio not found!" "Error"
        Write-Host "Please install Visual Studio with C++ development tools" -ForegroundColor Yellow
        Read-Host "Press Enter to exit"
        exit 1
    }
}

Write-Host ""
Write-Status "Step 2: Creating build directory..." "Info"
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Name "build" | Out-Null
}
Set-Location "build"

Write-Host ""
Write-Status "Step 3: Configuring with CMake..." "Info"

# Try different CMake configurations
$cmakeSuccess = $false

# Try with vcpkg if available
if (Test-Path "D:\vcpkg\scripts\buildsystems\vcpkg.cmake") {
    Write-Status "Attempting to configure with vcpkg..." "Warning"
    $cmakeResult = & cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake 2>&1
    if ($LASTEXITCODE -eq 0) {
        $cmakeSuccess = $true
        Write-Status "Configured with vcpkg" "Success"
    } else {
        if ($Verbose) {
            Write-Host $cmakeResult -ForegroundColor Red
        }
    }
}

# If vcpkg failed, try without it
if (-not $cmakeSuccess) {
    Write-Status "Attempting to configure without vcpkg..." "Warning"
    $cmakeResult = & cmake .. 2>&1
    if ($LASTEXITCODE -eq 0) {
        $cmakeSuccess = $true
        Write-Status "Configured successfully" "Success"
    } else {
        if ($Verbose) {
            Write-Host $cmakeResult -ForegroundColor Red
        }
    }
}

# If still failed, try with specific generator
if (-not $cmakeSuccess) {
    Write-Status "Attempting to configure with Visual Studio generator..." "Warning"
    $cmakeResult = & cmake .. -G "Visual Studio 17 2022" 2>&1
    if ($LASTEXITCODE -eq 0) {
        $cmakeSuccess = $true
        Write-Status "Configured with Visual Studio generator" "Success"
    } else {
        if ($Verbose) {
            Write-Host $cmakeResult -ForegroundColor Red
        }
    }
}

if (-not $cmakeSuccess) {
    Write-Host ""
    Write-Status "CMake configuration failed!" "Error"
    Write-Host ""
    Write-Host "Please make sure you have installed:" -ForegroundColor Yellow
    Write-Host "- Visual Studio with C++ development tools" -ForegroundColor Yellow
    Write-Host "- CMake" -ForegroundColor Yellow
    Write-Host "- OpenCV and Eigen3 (via vcpkg or manual installation)" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Installation instructions:" -ForegroundColor Cyan
    Write-Host "1. Install Visual Studio Community with 'Desktop development with C++'" -ForegroundColor White
    Write-Host "2. Install CMake from https://cmake.org/download/" -ForegroundColor White
    Write-Host "3. For dependencies, either:" -ForegroundColor White
    Write-Host "   a) Use vcpkg: git clone https://github.com/Microsoft/vcpkg.git" -ForegroundColor White
    Write-Host "      cd vcpkg && .\bootstrap-vcpkg.bat" -ForegroundColor White
    Write-Host "      .\vcpkg install opencv4 eigen3" -ForegroundColor White
    Write-Host "   b) Or install manually from their respective websites" -ForegroundColor White
    Write-Host ""
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host ""
Write-Status "Step 4: Building the project..." "Info"
$buildResult = & cmake --build . --config $BuildType 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Status "Build failed!" "Error"
    if ($Verbose) {
        Write-Host $buildResult -ForegroundColor Red
    }
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host ""
Write-Status "Step 5: Testing the build..." "Info"
if (-not (Test-Path "ai_detector.exe")) {
    Write-Status "ai_detector.exe not found after build." "Error"
    Write-Host "Checking for alternative executable names..." -ForegroundColor Yellow
    Get-ChildItem *.exe | ForEach-Object { Write-Host $_.Name -ForegroundColor Cyan }
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Status "Build successful! AI Detector is ready." "Success"

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "BUILD SUCCESSFUL!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""

Write-Status "Testing the help command:" "Info"
& .\ai_detector.exe help

Write-Host ""
Write-Host "========================================" -ForegroundColor Yellow
Write-Host "USAGE EXAMPLES:" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Yellow
Write-Host ""
Write-Host "To detect AI content in an image:" -ForegroundColor Green
Write-Host "  .\ai_detector.exe detect-image path\to\your\image.jpg" -ForegroundColor White
Write-Host ""
Write-Host "To detect AI content in a video:" -ForegroundColor Green
Write-Host "  .\ai_detector.exe detect-video path\to\your\video.mp4" -ForegroundColor White
Write-Host ""
Write-Host "To train a custom model:" -ForegroundColor Green
Write-Host "  .\ai_detector.exe train path\to\training\data\ output_model.bin" -ForegroundColor White
Write-Host ""
Write-Host "To use a specific model:" -ForegroundColor Green
Write-Host "  .\ai_detector.exe detect-image image.jpg my_model.bin" -ForegroundColor White
Write-Host ""

# Run demo if requested
if (-not $SkipDemo) {
    $runDemo = Read-Host "Would you like to run a demo test? (y/n)"
    if ($runDemo -eq "y" -or $runDemo -eq "Y") {
        Write-Host ""
        Write-Status "Running demo test..." "Info"
        Write-Host "Note: This will create a test image if none exists" -ForegroundColor Yellow
        
        # Check if demo.cpp exists and compile it
        if (Test-Path "..\demo.cpp") {
            Write-Status "Compiling demo program..." "Info"
            $demoResult = & cl /EHsc ..\demo.cpp /Fe:demo.exe 2>&1
            if ($LASTEXITCODE -eq 0) {
                Write-Status "Demo compiled successfully" "Success"
                Write-Status "Running demo..." "Info"
                & .\demo.exe
            } else {
                Write-Status "Demo compilation failed" "Error"
                if ($Verbose) {
                    Write-Host $demoResult -ForegroundColor Red
                }
            }
        } else {
            Write-Status "Demo file not found, skipping demo test" "Warning"
        }
    }
}

Write-Host ""
Write-Status "Project is ready to use!" "Success"
Write-Host "Press any key to exit..." -ForegroundColor Blue
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown") 