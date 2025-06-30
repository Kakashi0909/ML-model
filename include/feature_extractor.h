#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <Eigen/Dense>

class FeatureExtractor {
public:
    FeatureExtractor();
    ~FeatureExtractor() = default;

    // Extract features from an image
    Eigen::VectorXf extractFeatures(const cv::Mat& image);
    
    // Extract statistical features
    Eigen::VectorXf extractStatisticalFeatures(const cv::Mat& image);
    
    // Extract frequency domain features (FFT)
    Eigen::VectorXf extractFrequencyFeatures(const cv::Mat& image);
    
    // Extract texture features using GLCM
    Eigen::VectorXf extractTextureFeatures(const cv::Mat& image);
    
    // Extract noise analysis features
    Eigen::VectorXf extractNoiseFeatures(const cv::Mat& image);
    
    // Extract color distribution features
    Eigen::VectorXf extractColorFeatures(const cv::Mat& image);

private:
    // Helper methods
    cv::Mat preprocessImage(const cv::Mat& image);
    std::vector<float> calculateHistogram(const cv::Mat& image);
    std::vector<float> calculateGLCM(const cv::Mat& image);
    std::vector<float> calculateNoiseMetrics(const cv::Mat& image);
    
    // Configuration
    static constexpr int FEATURE_SIZE = 512;
    static constexpr int HISTOGRAM_BINS = 64;
    static constexpr int GLCM_DISTANCE = 1;
}; 