#include "../include/ai_detector.h"
#include <iostream>
#include <string>
#include <filesystem>

void printUsage() {
    std::cout << "AI Content Detector\n";
    std::cout << "Usage:\n";
    std::cout << "  ai_detector detect-image <image_path> [model_path]\n";
    std::cout << "  ai_detector detect-video <video_path> [model_path]\n";
    std::cout << "  ai_detector train <training_data_path> <output_model_path>\n";
    std::cout << "  ai_detector help\n\n";
    std::cout << "Commands:\n";
    std::cout << "  detect-image  - Detect AI-generated content in an image\n";
    std::cout << "  detect-video  - Detect AI-generated content in a video\n";
    std::cout << "  train         - Train the model with labeled data\n";
    std::cout << "  help          - Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  ai_detector detect-image sample.jpg\n";
    std::cout << "  ai_detector detect-video sample.mp4\n";
    std::cout << "  ai_detector detect-image sample.jpg model.bin\n";
    std::cout << "  ai_detector train training_data/ model.bin\n";
}

void detectImage(const std::string& image_path, const std::string& model_path = "") {
    AIDetector detector;
    
    if (!detector.initialize(model_path)) {
        std::cerr << "Failed to initialize detector" << std::endl;
        return;
    }
    
    std::cout << "Analyzing image: " << image_path << std::endl;
    float confidence = detector.detectImage(image_path);
    
    if (confidence < 0) {
        std::cerr << "Failed to analyze image" << std::endl;
        return;
    }
    
    std::cout << "AI Detection Confidence: " << (confidence * 100) << "%" << std::endl;
    
    if (confidence > 0.7f) {
        std::cout << "Result: Likely AI-generated content" << std::endl;
    } else if (confidence > 0.3f) {
        std::cout << "Result: Possibly AI-generated content" << std::endl;
    } else {
        std::cout << "Result: Likely real content" << std::endl;
    }
}

void detectVideo(const std::string& video_path, const std::string& model_path = "") {
    AIDetector detector;
    
    if (!detector.initialize(model_path)) {
        std::cerr << "Failed to initialize detector" << std::endl;
        return;
    }
    
    std::cout << "Analyzing video: " << video_path << std::endl;
    float confidence = detector.detectVideo(video_path);
    
    if (confidence < 0) {
        std::cerr << "Failed to analyze video" << std::endl;
        return;
    }
    
    std::cout << "AI Detection Confidence: " << (confidence * 100) << "%" << std::endl;
    
    if (confidence > 0.7f) {
        std::cout << "Result: Likely AI-generated content" << std::endl;
    } else if (confidence > 0.3f) {
        std::cout << "Result: Possibly AI-generated content" << std::endl;
    } else {
        std::cout << "Result: Likely real content" << std::endl;
    }
}

void trainModel(const std::string& training_data_path, const std::string& output_model_path) {
    AIDetector detector;
    
    std::cout << "Training model with data from: " << training_data_path << std::endl;
    std::cout << "Output model will be saved to: " << output_model_path << std::endl;
    
    if (!detector.train(training_data_path, output_model_path)) {
        std::cerr << "Failed to train model" << std::endl;
        return;
    }
    
    std::cout << "Training completed successfully!" << std::endl;
    std::cout << "Model saved to: " << output_model_path << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }
    
    std::string command = argv[1];
    
    try {
        if (command == "detect-image") {
            if (argc < 3) {
                std::cerr << "Error: Image path required" << std::endl;
                printUsage();
                return 1;
            }
            
            std::string image_path = argv[2];
            std::string model_path = (argc > 3) ? argv[3] : "";
            
            if (!std::filesystem::exists(image_path)) {
                std::cerr << "Error: Image file not found: " << image_path << std::endl;
                return 1;
            }
            
            detectImage(image_path, model_path);
            
        } else if (command == "detect-video") {
            if (argc < 3) {
                std::cerr << "Error: Video path required" << std::endl;
                printUsage();
                return 1;
            }
            
            std::string video_path = argv[2];
            std::string model_path = (argc > 3) ? argv[3] : "";
            
            if (!std::filesystem::exists(video_path)) {
                std::cerr << "Error: Video file not found: " << video_path << std::endl;
                return 1;
            }
            
            detectVideo(video_path, model_path);
            
        } else if (command == "train") {
            if (argc < 4) {
                std::cerr << "Error: Training data path and output model path required" << std::endl;
                printUsage();
                return 1;
            }
            
            std::string training_data_path = argv[2];
            std::string output_model_path = argv[3];
            
            if (!std::filesystem::exists(training_data_path)) {
                std::cerr << "Error: Training data path not found: " << training_data_path << std::endl;
                return 1;
            }
            
            trainModel(training_data_path, output_model_path);
            
        } else if (command == "help") {
            printUsage();
            
        } else {
            std::cerr << "Error: Unknown command '" << command << "'" << std::endl;
            printUsage();
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 