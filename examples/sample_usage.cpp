#include "../include/ai_detector.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "AI Content Detector - Sample Usage\n";
    std::cout << "==================================\n\n";
    
    // Create detector instance
    AIDetector detector;
    
    // Initialize detector (without pre-trained model for this example)
    if (!detector.initialize()) {
        std::cerr << "Failed to initialize detector" << std::endl;
        return 1;
    }
    
    std::cout << "Detector initialized successfully!\n\n";
    
    // Example 1: Create a synthetic image for testing
    std::cout << "Example 1: Creating synthetic test image...\n";
    cv::Mat test_image = cv::Mat::zeros(224, 224, CV_8UC3);
    
    // Add some synthetic patterns (simulating AI-generated content)
    for (int i = 0; i < test_image.rows; i += 10) {
        for (int j = 0; j < test_image.cols; j += 10) {
            cv::rectangle(test_image, cv::Point(j, i), cv::Point(j + 8, i + 8), 
                         cv::Scalar(rand() % 255, rand() % 255, rand() % 255), -1);
        }
    }
    
    // Add some noise to make it more realistic
    cv::Mat noise = cv::Mat::zeros(224, 224, CV_8UC3);
    cv::randn(noise, cv::Scalar::all(0), cv::Scalar::all(25));
    test_image += noise;
    
    // Analyze the synthetic image
    float confidence = detector.detectImage(test_image);
    std::cout << "Synthetic image AI confidence: " << (confidence * 100) << "%\n";
    
    if (confidence > 0.7f) {
        std::cout << "Result: Likely AI-generated content\n";
    } else if (confidence > 0.3f) {
        std::cout << "Result: Possibly AI-generated content\n";
    } else {
        std::cout << "Result: Likely real content\n";
    }
    std::cout << "\n";
    
    // Example 2: Create a more natural-looking image
    std::cout << "Example 2: Creating natural test image...\n";
    cv::Mat natural_image = cv::Mat::zeros(224, 224, CV_8UC3);
    
    // Create a gradient background
    for (int i = 0; i < natural_image.rows; ++i) {
        for (int j = 0; j < natural_image.cols; ++j) {
            int blue = (i * 255) / natural_image.rows;
            int green = (j * 255) / natural_image.cols;
            int red = ((i + j) * 255) / (natural_image.rows + natural_image.cols);
            natural_image.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);
        }
    }
    
    // Add some natural variations
    cv::Mat natural_noise = cv::Mat::zeros(224, 224, CV_8UC3);
    cv::randn(natural_noise, cv::Scalar::all(0), cv::Scalar::all(15));
    natural_image += natural_noise;
    
    // Analyze the natural image
    confidence = detector.detectImage(natural_image);
    std::cout << "Natural image AI confidence: " << (confidence * 100) << "%\n";
    
    if (confidence > 0.7f) {
        std::cout << "Result: Likely AI-generated content\n";
    } else if (confidence > 0.3f) {
        std::cout << "Result: Possibly AI-generated content\n";
    } else {
        std::cout << "Result: Likely real content\n";
    }
    std::cout << "\n";
    
    // Example 3: Demonstrate feature extraction
    std::cout << "Example 3: Feature extraction demonstration...\n";
    
    // Create a feature extractor
    FeatureExtractor extractor;
    
    // Extract features from the synthetic image
    Eigen::VectorXf features = extractor.extractFeatures(test_image);
    std::cout << "Extracted " << features.size() << " features\n";
    
    // Show some feature statistics
    float mean_feature = features.mean();
    float std_feature = std::sqrt((features.array() - mean_feature).square().mean());
    
    std::cout << "Feature statistics:\n";
    std::cout << "  Mean: " << mean_feature << "\n";
    std::cout << "  Std Dev: " << std_feature << "\n";
    std::cout << "  Min: " << features.minCoeff() << "\n";
    std::cout << "  Max: " << features.maxCoeff() << "\n\n";
    
    // Example 4: Neural network demonstration
    std::cout << "Example 4: Neural network demonstration...\n";
    
    // Create a simple neural network
    NeuralNetwork nn;
    std::vector<int> layer_sizes = {512, 128, 64, 1};
    nn.initialize(layer_sizes);
    
    // Make a prediction
    float prediction = nn.predict(features);
    std::cout << "Neural network prediction: " << prediction << "\n";
    std::cout << "Interpreted as: " << (prediction > 0.5 ? "AI-generated" : "Real") << "\n\n";
    
    // Example 5: Training demonstration
    std::cout << "Example 5: Training demonstration...\n";
    
    // Create some synthetic training data
    std::vector<Eigen::VectorXf> training_inputs;
    std::vector<Eigen::VectorXf> training_targets;
    
    for (int i = 0; i < 100; ++i) {
        // Generate random features
        Eigen::VectorXf input = Eigen::VectorXf::Random(512);
        
        // Create target (alternate between AI and real)
        Eigen::VectorXf target(1);
        target(0) = (i % 2 == 0) ? 1.0f : 0.0f;
        
        training_inputs.push_back(input);
        training_targets.push_back(target);
    }
    
    std::cout << "Training neural network with " << training_inputs.size() << " samples...\n";
    nn.train(training_inputs, training_targets, 0.01f, 10);
    
    // Test the trained network
    float trained_prediction = nn.predict(features);
    std::cout << "Trained network prediction: " << trained_prediction << "\n";
    std::cout << "Interpreted as: " << (trained_prediction > 0.5 ? "AI-generated" : "Real") << "\n\n";
    
    // Example 6: Model saving/loading
    std::cout << "Example 6: Model persistence demonstration...\n";
    
    // Save the trained model
    if (nn.saveModel("test_model.bin")) {
        std::cout << "Model saved successfully\n";
        
        // Load the model into a new network
        NeuralNetwork loaded_nn;
        if (loaded_nn.loadModel("test_model.bin")) {
            std::cout << "Model loaded successfully\n";
            
            // Test the loaded model
            float loaded_prediction = loaded_nn.predict(features);
            std::cout << "Loaded model prediction: " << loaded_prediction << "\n";
            
            // Verify predictions match
            if (std::abs(trained_prediction - loaded_prediction) < 1e-6) {
                std::cout << "Predictions match - model persistence working correctly!\n";
            } else {
                std::cout << "Warning: Predictions don't match\n";
            }
        } else {
            std::cout << "Failed to load model\n";
        }
    } else {
        std::cout << "Failed to save model\n";
    }
    
    std::cout << "\nSample usage completed successfully!\n";
    return 0;
} 