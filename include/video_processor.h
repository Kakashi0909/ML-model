#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "feature_extractor.h"

class VideoProcessor {
public:
    VideoProcessor();
    ~VideoProcessor() = default;

    // Process video and return AI detection confidence
    float processVideo(const std::string& video_path);
    
    // Extract frames from video
    std::vector<cv::Mat> extractFrames(const std::string& video_path, int max_frames = 30);
    
    // Process individual frames
    std::vector<float> processFrames(const std::vector<cv::Mat>& frames);
    
    // Temporal analysis
    float analyzeTemporalConsistency(const std::vector<cv::Mat>& frames);
    
    // Motion analysis
    float analyzeMotionPatterns(const std::vector<cv::Mat>& frames);

private:
    std::unique_ptr<FeatureExtractor> feature_extractor_;
    
    // Helper methods
    cv::Mat calculateOpticalFlow(const cv::Mat& frame1, const cv::Mat& frame2);
    std::vector<float> calculateFrameDifferences(const std::vector<cv::Mat>& frames);
    float calculateTemporalVariance(const std::vector<cv::Mat>& frames);
    
    // Configuration
    static constexpr int MAX_FRAMES = 30;
    static constexpr float FRAME_SAMPLE_RATE = 1.0f; // Extract every frame
    static constexpr int MIN_FRAME_SIZE = 224;
}; 