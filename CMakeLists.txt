cmake_minimum_required(VERSION 3.16)
project(AIDetector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

# Add source files
set(SOURCES
    src/main.cpp
    src/ai_detector.cpp
    src/feature_extractor.cpp
    src/neural_network.cpp
    src/video_processor.cpp
)

# Create executable
add_executable(ai_detector ${SOURCES})

# Link libraries
target_link_libraries(ai_detector ${OpenCV_LIBS} Eigen3::Eigen)

# Set compiler flags
if(MSVC)
    target_compile_options(ai_detector PRIVATE /W4)
else()
    target_compile_options(ai_detector PRIVATE -Wall -Wextra -O3)
endif() 