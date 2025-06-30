# AI Content Detector

A C++ machine learning model for detecting AI-generated images and videos using computer vision techniques and neural networks.

## Features

- **Image Analysis**: Detects AI-generated content in images using multiple feature extraction methods
- **Video Analysis**: Analyzes videos for AI-generated content with temporal consistency and motion pattern analysis
- **Feature Extraction**: Comprehensive feature extraction including:
  - Statistical features (mean, variance, histograms)
  - Frequency domain features (FFT analysis)
  - Texture features (GLCM - Gray Level Co-occurrence Matrix)
  - Noise analysis features
  - Color distribution features
- **Neural Network**: Custom implementation with:
  - Multi-layer perceptron architecture
  - ReLU activation for hidden layers
  - Sigmoid activation for output layer
  - Binary cross-entropy loss function
  - Gradient descent optimization
- **Model Persistence**: Save and load trained models
- **Command-line Interface**: Easy-to-use CLI for detection and training

## Technical Details

### Feature Extraction Methods

1. **Statistical Features**: Basic image statistics, histogram analysis
2. **Frequency Domain**: FFT-based analysis to detect artificial patterns
3. **Texture Analysis**: GLCM features to identify synthetic textures
4. **Noise Analysis**: Laplacian variance and noise pattern analysis
5. **Color Analysis**: Color space analysis and distribution features

### Neural Network Architecture

- Input layer: 512 features
- Hidden layers: 256 → 128 → 64 neurons
- Output layer: 1 neuron (binary classification)
- Activation functions: ReLU (hidden), Sigmoid (output)
- Loss function: Binary Cross-Entropy

### Video Analysis

- Frame extraction with temporal sampling
- Optical flow analysis for motion patterns
- Temporal consistency analysis
- Combined scoring from multiple analysis methods

## Dependencies

- **OpenCV 4.x**: Computer vision library
- **Eigen3**: Linear algebra library
- **C++17**: Modern C++ features
- **CMake**: Build system

## Installation

### Prerequisites

1. **OpenCV**: Install OpenCV development libraries
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libopencv-dev
   
   # macOS
   brew install opencv
   
   # Windows
   # Download from https://opencv.org/releases/
   ```

2. **Eigen3**: Install Eigen3 library
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libeigen3-dev
   
   # macOS
   brew install eigen
   
   # Windows
   # Download from https://eigen.tuxfamily.org/
   ```

3. **CMake**: Install CMake
   ```bash
   # Ubuntu/Debian
   sudo apt-get install cmake
   
   # macOS
   brew install cmake
   
   # Windows
   # Download from https://cmake.org/download/
   ```

### Building the Project

1. Clone or download the project
2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure with CMake:
   ```bash
   cmake ..
   ```

4. Build the project:
   ```bash
   make
   ```

5. The executable `ai_detector` will be created in the build directory.

## Usage

### Command Line Interface

The AI detector provides a simple command-line interface:

```bash
./ai_detector <command> [arguments]
```

### Commands

#### Detect AI-generated content in an image:
```bash
./ai_detector detect-image <image_path> [model_path]
```

#### Detect AI-generated content in a video:
```bash
./ai_detector detect-video <video_path> [model_path]
```

#### Train the model:
```bash
./ai_detector train <training_data_path> <output_model_path>
```

#### Show help:
```bash
./ai_detector help
```

### Examples

```bash
# Detect AI content in an image
./ai_detector detect-image sample.jpg

# Detect AI content in a video
./ai_detector detect-video sample.mp4

# Use a specific trained model
./ai_detector detect-image sample.jpg my_model.bin

# Train a new model
./ai_detector train training_data/ model.bin
```

### Output Interpretation

The detector outputs a confidence score between 0% and 100%:

- **0-30%**: Likely real content
- **30-70%**: Possibly AI-generated content
- **70-100%**: Likely AI-generated content

## Training Data Format

For training, you need to organize your data as follows:

```
training_data/
├── real/
│   ├── image1.jpg
│   ├── image2.png
│   └── ...
└── ai_generated/
    ├── ai_image1.jpg
    ├── ai_image2.png
    └── ...
```

## Technical Implementation

### Feature Extraction Pipeline

1. **Preprocessing**: Resize to 224x224, normalize to [0,1]
2. **Statistical Analysis**: Mean, variance, histogram features
3. **Frequency Analysis**: FFT magnitude spectrum analysis
4. **Texture Analysis**: GLCM features in multiple directions
5. **Noise Analysis**: Laplacian variance and noise pattern detection
6. **Color Analysis**: Multi-color space histogram analysis

### Neural Network Training

- **Optimization**: Stochastic Gradient Descent
- **Learning Rate**: Configurable (default: 0.01)
- **Epochs**: Configurable (default: 100)
- **Loss Function**: Binary Cross-Entropy
- **Regularization**: Built-in through early stopping

### Video Analysis Pipeline

1. **Frame Extraction**: Sample frames at regular intervals
2. **Individual Frame Analysis**: Apply image detection to each frame
3. **Temporal Analysis**: Analyze frame-to-frame consistency
4. **Motion Analysis**: Optical flow analysis for motion patterns
5. **Score Combination**: Weighted average of all analysis results

## Limitations

- Requires sufficient training data for accurate results
- Performance depends on image/video quality
- May have false positives/negatives
- Training requires labeled dataset
- Computational requirements increase with video length

## Future Improvements

- Support for more image/video formats
- GPU acceleration using CUDA
- Real-time detection capabilities
- Web interface
- API for integration with other applications
- Support for different AI generation models
- Ensemble methods for improved accuracy

## License

This project is provided as-is for educational and research purposes.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## Disclaimer

This tool is designed for educational and research purposes. The accuracy of AI detection depends on many factors including the quality of training data and the specific AI generation methods being detected. Results should not be considered definitive proof of content authenticity. 