#include "../include/ai_detector.h"
#include <iostream>
#include <fstream>

AIDetector::AIDetector() : is_initialized_(false) {
    feature_extractor_ = std::make_unique<FeatureExtractor>();
    neural_network_ = std::make_unique<NeuralNetwork>();
    video_processor_ = std::make_unique<VideoProcessor>();
}

bool AIDetector::initialize(const std::string& model_path) {
    if (!model_path.empty()) {
        if (!loadModel(model_path)) {
            std::cerr << "Failed to load model from: " << model_path << std::endl;
            return false;
        }
    } else {
        // Initialize with default architecture
        std::vector<int> layer_sizes = {512, 256, 128, 64, 1};
        neural_network_->initialize(layer_sizes);
    }
    
    is_initialized_ = true;
    std::cout << "AI Detector initialized successfully" << std::endl;
    return true;
}

float AIDetector::detectImage(const std::string& image_path) {
    cv::Mat image = cv::imread(image_path);
    if (image.empty()) {
        std::cerr << "Failed to load image: " << image_path << std::endl;
        return -1.0f;
    }
    return detectImage(image);
}

float AIDetector::detectImage(const cv::Mat& image) {
    if (!is_initialized_) {
        std::cerr << "Detector not initialized. Call initialize() first." << std::endl;
        return -1.0f;
    }
    
    // Extract features
    Eigen::VectorXf features = feature_extractor_->extractFeatures(image);
    
    // Make prediction
    float confidence = neural_network_->predict(features);
    
    return confidence;
}

float AIDetector::detectVideo(const std::string& video_path) {
    if (!is_initialized_) {
        std::cerr << "Detector not initialized. Call initialize() first." << std::endl;
        return -1.0f;
    }
    
    return video_processor_->processVideo(video_path);
}

bool AIDetector::train(const std::string& training_data_path, const std::string& output_model_path) {
    // This is a simplified training implementation
    // In a real scenario, you would load training data from files
    std::cout << "Training model..." << std::endl;
    
    // For demonstration, we'll create some synthetic training data
    std::vector<Eigen::VectorXf> inputs;
    std::vector<Eigen::VectorXf> targets;
    
    // Generate synthetic training data
    for (int i = 0; i < 1000; ++i) {
        Eigen::VectorXf input = Eigen::VectorXf::Random(512);
        Eigen::VectorXf target(1);
        target(0) = (i % 2 == 0) ? 1.0f : 0.0f; // Alternate between AI and real
        inputs.push_back(input);
        targets.push_back(target);
    }
    
    // Train the network
    neural_network_->train(inputs, targets, 0.01f, 50);
    
    // Save the trained model
    if (!output_model_path.empty()) {
        return saveModel(output_model_path);
    }
    
    return true;
}

bool AIDetector::saveModel(const std::string& model_path) {
    return neural_network_->saveModel(model_path);
}

bool AIDetector::loadModel(const std::string& model_path) {
    return neural_network_->loadModel(model_path);
} 