# AI Content Detector - Dependency Installation Script
# This script helps install the required dependencies

Write-Host "========================================" -ForegroundColor Green
Write-Host "AI Content Detector - Dependency Setup" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""

# Check if running as administrator
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")

if (-not $isAdmin) {
    Write-Host "Warning: This script is not running as administrator." -ForegroundColor Yellow
    Write-Host "Some installations may fail. Consider running as administrator." -ForegroundColor Yellow
    Write-Host ""
}

# Function to check if a command exists
function Test-Command($cmdname) {
    return [bool](Get-Command -Name $cmdname -ErrorAction SilentlyContinue)
}

# Check current status
Write-Host "Checking current installation status..." -ForegroundColor Cyan

$cmakeInstalled = Test-Command "cmake"
$gitInstalled = Test-Command "git"
$vcpkgExists = Test-Path "D:\vcpkg\vcpkg.exe"

Write-Host "CMake installed: $cmakeInstalled" -ForegroundColor $(if ($cmakeInstalled) { "Green" } else { "Red" })
Write-Host "Git installed: $gitInstalled" -ForegroundColor $(if ($gitInstalled) { "Green" } else { "Red" })
Write-Host "vcpkg available: $vcpkgExists" -ForegroundColor $(if ($vcpkgExists) { "Green" } else { "Red" })
Write-Host ""

# Installation options
Write-Host "Installation Options:" -ForegroundColor Yellow
Write-Host "1. Install CMake (if not already installed)"
Write-Host "2. Install vcpkg and dependencies (OpenCV, Eigen3)"
Write-Host "3. Check Visual Studio installation"
Write-Host "4. Build the project (requires all dependencies)"
Write-Host "5. Exit"
Write-Host ""

$choice = Read-Host "Enter your choice (1-5)"

switch ($choice) {
    "1" {
        Write-Host "Installing CMake..." -ForegroundColor Cyan
        if (Test-Command "choco") {
            Write-Host "Using Chocolatey to install CMake..." -ForegroundColor Green
            choco install cmake -y
        } else {
            Write-Host "Chocolatey not found. Please install CMake manually:" -ForegroundColor Yellow
            Write-Host "1. Download from https://cmake.org/download/" -ForegroundColor White
            Write-Host "2. Install and add to PATH" -ForegroundColor White
        }
    }
    "2" {
        Write-Host "Setting up vcpkg and dependencies..." -ForegroundColor Cyan
        
        if (-not $gitInstalled) {
            Write-Host "Git not found. Please install Git first:" -ForegroundColor Red
            Write-Host "Download from https://git-scm.com/download/win" -ForegroundColor White
            break
        }
        
        if (-not $vcpkgExists) {
            Write-Host "Installing vcpkg..." -ForegroundColor Green
            Set-Location "D:\"
            git clone https://github.com/Microsoft/vcpkg.git
            Set-Location "D:\vcpkg"
            .\bootstrap-vcpkg.bat
            Set-Location "D:\task"
        }
        
        Write-Host "Installing OpenCV and Eigen3..." -ForegroundColor Green
        Set-Location "D:\vcpkg"
        .\vcpkg install opencv4 eigen3
        Set-Location "D:\task"
    }
    "3" {
        Write-Host "Checking Visual Studio installation..." -ForegroundColor Cyan
        
        $vsPaths = @(
            "C:\Program Files\Microsoft Visual Studio",
            "C:\Program Files (x86)\Microsoft Visual Studio"
        )
        
        $vsFound = $false
        foreach ($path in $vsPaths) {
            if (Test-Path $path) {
                $installations = Get-ChildItem $path -Directory | Where-Object { $_.Name -match "^\d{4}$" }
                if ($installations) {
                    Write-Host "Visual Studio found at: $path" -ForegroundColor Green
                    foreach ($installation in $installations) {
                        Write-Host "  - Version: $($installation.Name)" -ForegroundColor White
                    }
                    $vsFound = $true
                }
            }
        }
        
        if (-not $vsFound) {
            Write-Host "Visual Studio not found!" -ForegroundColor Red
            Write-Host "Please install Visual Studio Community with 'Desktop development with C++' workload:" -ForegroundColor Yellow
            Write-Host "https://visualstudio.microsoft.com/vs/community/" -ForegroundColor White
        }
    }
    "4" {
        Write-Host "Building the project..." -ForegroundColor Cyan
        
        if (-not $cmakeInstalled) {
            Write-Host "CMake not installed. Please install it first." -ForegroundColor Red
            break
        }
        
        if (-not $vcpkgExists) {
            Write-Host "vcpkg not found. Please install it first." -ForegroundColor Red
            break
        }
        
        Write-Host "Running build script..." -ForegroundColor Green
        .\build_and_run.bat
    }
    "5" {
        Write-Host "Exiting..." -ForegroundColor Green
        exit
    }
    default {
        Write-Host "Invalid choice. Please enter 1-5." -ForegroundColor Red
    }
}

Write-Host ""
Write-Host "Script completed. Check the output above for any errors." -ForegroundColor Green
Write-Host "Press any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown") 