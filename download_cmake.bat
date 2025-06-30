@echo off
echo Downloading CMake installer...
powershell -Command "& {Invoke-WebRequest -Uri 'https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1-windows-x86_64.msi' -OutFile 'cmake-installer.msi'}"
echo CMake installer downloaded successfully!
echo Please run: msiexec /i cmake-installer.msi /quiet
pause 