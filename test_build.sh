#!/bin/bash

echo "AI Content Detector - Build Test"
echo "================================"

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found. Please run this script from the project root."
    exit 1
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
if ! cmake ..; then
    echo "Error: CMake configuration failed."
    exit 1
fi

# Build the project
echo "Building project..."
if ! make; then
    echo "Error: Build failed."
    exit 1
fi

# Check if executable was created
if [ ! -f "ai_detector" ]; then
    echo "Error: Executable 'ai_detector' not found after build."
    exit 1
fi

echo "Build successful! Executable created: ai_detector"

# Test basic functionality
echo ""
echo "Testing basic functionality..."

# Test help command
echo "Testing help command..."
if ! ./ai_detector help > /dev/null 2>&1; then
    echo "Error: Help command failed."
    exit 1
fi

echo "Help command works correctly."

# Test with invalid command
echo "Testing invalid command handling..."
if ./ai_detector invalid_command > /dev/null 2>&1; then
    echo "Error: Invalid command should have failed."
    exit 1
fi

echo "Invalid command handling works correctly."

# Test with missing arguments
echo "Testing missing arguments handling..."
if ./ai_detector detect-image > /dev/null 2>&1; then
    echo "Error: Missing arguments should have failed."
    exit 1
fi

echo "Missing arguments handling works correctly."

echo ""
echo "All tests passed! The AI detector is ready to use."
echo ""
echo "Usage examples:"
echo "  ./ai_detector help"
echo "  ./ai_detector detect-image <image_path>"
echo "  ./ai_detector detect-video <video_path>"
echo "  ./ai_detector train <training_data_path> <output_model_path>" 