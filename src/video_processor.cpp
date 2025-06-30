#include "../include/video_processor.h"
#include <iostream>
#include <algorithm>

VideoProcessor::VideoProcessor() {
    feature_extractor_ = std::make_unique<FeatureExtractor>();
}

float VideoProcessor::processVideo(const std::string& video_path) {
    // Extract frames from video
    std::vector<cv::Mat> frames = extractFrames(video_path);
    
    if (frames.empty()) {
        std::cerr << "No frames extracted from video: " << video_path << std::endl;
        return -1.0f;
    }
    
    // Process individual frames
    std::vector<float> frame_scores = processFrames(frames);
    
    // Analyze temporal consistency
    float temporal_score = analyzeTemporalConsistency(frames);
    
    // Analyze motion patterns
    float motion_score = analyzeMotionPatterns(frames);
    
    // Combine scores (weighted average)
    float avg_frame_score = 0.0f;
    for (float score : frame_scores) {
        avg_frame_score += score;
    }
    avg_frame_score /= frame_scores.size();
    
    // Final score combines frame analysis, temporal consistency, and motion patterns
    float final_score = 0.6f * avg_frame_score + 0.2f * temporal_score + 0.2f * motion_score;
    
    return final_score;
}

std::vector<cv::Mat> VideoProcessor::extractFrames(const std::string& video_path, int max_frames) {
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        std::cerr << "Failed to open video: " << video_path << std::endl;
        return {};
    }
    
    std::vector<cv::Mat> frames;
    int total_frames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    int frame_interval = std::max(1, total_frames / max_frames);
    
    cv::Mat frame;
    int frame_count = 0;
    
    while (cap.read(frame) && frames.size() < static_cast<size_t>(max_frames)) {
        if (frame_count % frame_interval == 0) {
            // Resize frame to minimum size
            if (frame.rows < MIN_FRAME_SIZE || frame.cols < MIN_FRAME_SIZE) {
                cv::resize(frame, frame, cv::Size(MIN_FRAME_SIZE, MIN_FRAME_SIZE));
            }
            frames.push_back(frame.clone());
        }
        frame_count++;
    }
    
    cap.release();
    return frames;
}

std::vector<float> VideoProcessor::processFrames(const std::vector<cv::Mat>& frames) {
    std::vector<float> scores;
    
    for (const auto& frame : frames) {
        // Extract features from frame
        Eigen::VectorXf features = feature_extractor_->extractFeatures(frame);
        
        // For now, we'll use a simple heuristic based on feature statistics
        // In a real implementation, you'd use the trained neural network
        
        // Calculate feature statistics
        float mean = features.mean();
        float stddev = std::sqrt((features.array() - mean).square().mean());
        
        // Simple heuristic: AI-generated content often has more uniform feature distributions
        float uniformity_score = 1.0f - std::min(stddev, 1.0f);
        
        scores.push_back(uniformity_score);
    }
    
    return scores;
}

float VideoProcessor::analyzeTemporalConsistency(const std::vector<cv::Mat>& frames) {
    if (frames.size() < 2) {
        return 0.5f; // Neutral score for single frame
    }
    
    std::vector<float> frame_differences = calculateFrameDifferences(frames);
    
    // Calculate temporal variance
    float temporal_variance = calculateTemporalVariance(frames);
    
    // AI-generated videos often have more consistent frame-to-frame differences
    // Real videos typically have more natural variation
    float consistency_score = 1.0f - std::min(temporal_variance, 1.0f);
    
    return consistency_score;
}

float VideoProcessor::analyzeMotionPatterns(const std::vector<cv::Mat>& frames) {
    if (frames.size() < 2) {
        return 0.5f; // Neutral score for single frame
    }
    
    std::vector<float> motion_scores;
    
    for (size_t i = 1; i < frames.size(); ++i) {
        cv::Mat flow = calculateOpticalFlow(frames[i-1], frames[i]);
        
        // Calculate motion magnitude
        cv::Mat magnitude, angle;
        std::vector<cv::Mat> flow_parts;
        cv::split(flow, flow_parts);
        cv::cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle);
        
        // Calculate motion statistics
        cv::Scalar mean_magnitude = cv::mean(magnitude);
        cv::Scalar stddev_magnitude;
        cv::meanStdDev(magnitude, cv::noArray(), stddev_magnitude);
        
        // AI-generated videos often have more uniform motion patterns
        float motion_uniformity = 1.0f - std::min(stddev_magnitude[0] / (mean_magnitude[0] + 1e-6f), 1.0f);
        motion_scores.push_back(motion_uniformity);
    }
    
    // Average motion score
    float avg_motion_score = 0.0f;
    for (float score : motion_scores) {
        avg_motion_score += score;
    }
    avg_motion_score /= motion_scores.size();
    
    return avg_motion_score;
}

cv::Mat VideoProcessor::calculateOpticalFlow(const cv::Mat& frame1, const cv::Mat& frame2) {
    cv::Mat gray1, gray2;
    
    if (frame1.channels() == 3) {
        cv::cvtColor(frame1, gray1, cv::COLOR_BGR2GRAY);
    } else {
        gray1 = frame1.clone();
    }
    
    if (frame2.channels() == 3) {
        cv::cvtColor(frame2, gray2, cv::COLOR_BGR2GRAY);
    } else {
        gray2 = frame2.clone();
    }
    
    // Calculate optical flow using Farneback method
    cv::Mat flow;
    cv::calcOpticalFlowFarneback(gray1, gray2, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
    
    return flow;
}

std::vector<float> VideoProcessor::calculateFrameDifferences(const std::vector<cv::Mat>& frames) {
    std::vector<float> differences;
    
    for (size_t i = 1; i < frames.size(); ++i) {
        cv::Mat diff;
        cv::absdiff(frames[i-1], frames[i], diff);
        
        // Convert to grayscale if needed
        if (diff.channels() == 3) {
            cv::cvtColor(diff, diff, cv::COLOR_BGR2GRAY);
        }
        
        // Calculate mean difference
        cv::Scalar mean_diff = cv::mean(diff);
        differences.push_back(mean_diff[0] / 255.0f);
    }
    
    return differences;
}

float VideoProcessor::calculateTemporalVariance(const std::vector<cv::Mat>& frames) {
    if (frames.size() < 2) {
        return 0.0f;
    }
    
    std::vector<float> differences = calculateFrameDifferences(frames);
    
    // Calculate variance of frame differences
    float mean_diff = 0.0f;
    for (float diff : differences) {
        mean_diff += diff;
    }
    mean_diff /= differences.size();
    
    float variance = 0.0f;
    for (float diff : differences) {
        variance += (diff - mean_diff) * (diff - mean_diff);
    }
    variance /= differences.size();
    
    return variance;
} 