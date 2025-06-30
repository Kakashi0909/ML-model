#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include "feature_extractor.h"
#include "neural_network.h"
#include "video_processor.h"

class AIDetector {
public:
    AIDetector();
    ~AIDetector() = default;

    // Initialize the detector with a pre-trained model
    bool initialize(const std::string& model_path = "");
    
    // Detect AI-generated content in an image
    float detectImage(const std::string& image_path);
    float detectImage(const cv::Mat& image);
    
    // Detect AI-generated content in a video
    float detectVideo(const std::string& video_path);
    
    // Train the model with labeled data
    bool train(const std::string& training_data_path, const std::string& output_model_path);
    
    // Save/load model
    bool saveModel(const std::string& model_path);
    bool loadModel(const std::string& model_path);

private:
    std::unique_ptr<FeatureExtractor> feature_extractor_;
    std::unique_ptr<NeuralNetwork> neural_network_;
    std::unique_ptr<VideoProcessor> video_processor_;
    
    bool is_initialized_;
    
    // Configuration parameters
    static constexpr int INPUT_SIZE = 224;
    static constexpr float CONFIDENCE_THRESHOLD = 0.5f;
}; 