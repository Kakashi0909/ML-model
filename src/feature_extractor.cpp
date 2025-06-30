#include "../include/feature_extractor.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/core/eigen.hpp>
#include <cmath>
#include <algorithm>

FeatureExtractor::FeatureExtractor() = default;

Eigen::VectorXf FeatureExtractor::extractFeatures(const cv::Mat& image) {
    cv::Mat processed = preprocessImage(image);
    
    // Combine all feature types
    Eigen::VectorXf statistical = extractStatisticalFeatures(processed);
    Eigen::VectorXf frequency = extractFrequencyFeatures(processed);
    Eigen::VectorXf texture = extractTextureFeatures(processed);
    Eigen::VectorXf noise = extractNoiseFeatures(processed);
    Eigen::VectorXf color = extractColorFeatures(processed);
    
    // Concatenate all features
    Eigen::VectorXf combined(FEATURE_SIZE);
    int offset = 0;
    
    // Statistical features (64 features)
    combined.segment(offset, statistical.size()) = statistical;
    offset += statistical.size();
    
    // Frequency features (128 features)
    combined.segment(offset, frequency.size()) = frequency;
    offset += frequency.size();
    
    // Texture features (128 features)
    combined.segment(offset, texture.size()) = texture;
    offset += texture.size();
    
    // Noise features (128 features)
    combined.segment(offset, noise.size()) = noise;
    offset += noise.size();
    
    // Color features (64 features)
    combined.segment(offset, color.size()) = color;
    
    return combined;
}

Eigen::VectorXf FeatureExtractor::extractStatisticalFeatures(const cv::Mat& image) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }
    
    Eigen::VectorXf features(64);
    
    // Basic statistics
    cv::Scalar mean, stddev;
    cv::meanStdDev(gray, mean, stddev);
    
    features(0) = mean[0] / 255.0f;
    features(1) = stddev[0] / 255.0f;
    
    // Histogram features
    std::vector<float> hist = calculateHistogram(gray);
    for (int i = 0; i < std::min(62, (int)hist.size()); ++i) {
        features(i + 2) = hist[i];
    }
    
    return features;
}

Eigen::VectorXf FeatureExtractor::extractFrequencyFeatures(const cv::Mat& image) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }
    
    // Resize to power of 2 for FFT
    cv::Mat resized;
    cv::resize(gray, resized, cv::Size(256, 256));
    
    // Convert to float
    cv::Mat float_img;
    resized.convertTo(float_img, CV_32F);
    
    // Apply FFT
    cv::Mat complex_img;
    cv::dft(float_img, complex_img, cv::DFT_COMPLEX_OUTPUT);
    
    // Calculate magnitude spectrum
    cv::Mat magnitude;
    cv::magnitude(complex_img.reshape(2), cv::Mat::zeros(256, 256, CV_32F), magnitude);
    
    // Log scale
    magnitude += cv::Scalar::all(1);
    cv::log(magnitude, magnitude);
    
    // Extract features from magnitude spectrum
    Eigen::VectorXf features(128);
    
    // Low frequency features (center region)
    cv::Rect center_region(96, 96, 64, 64);
    cv::Mat center_mag = magnitude(center_region);
    cv::Scalar center_mean = cv::mean(center_mag);
    features(0) = center_mean[0];
    
    // High frequency features (corners)
    std::vector<cv::Rect> corner_regions = {
        cv::Rect(0, 0, 32, 32),
        cv::Rect(224, 0, 32, 32),
        cv::Rect(0, 224, 32, 32),
        cv::Rect(224, 224, 32, 32)
    };
    
    for (int i = 0; i < 4; ++i) {
        cv::Mat corner_mag = magnitude(corner_regions[i]);
        cv::Scalar corner_mean = cv::mean(corner_mag);
        features(i + 1) = corner_mean[0];
    }
    
    // Frequency bands
    for (int i = 0; i < 123; ++i) {
        int x = (i * 7) % 256;
        int y = (i * 11) % 256;
        features(i + 5) = magnitude.at<float>(y, x);
    }
    
    return features;
}

Eigen::VectorXf FeatureExtractor::extractTextureFeatures(const cv::Mat& image) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }
    
    // Resize for GLCM calculation
    cv::Mat resized;
    cv::resize(gray, resized, cv::Size(128, 128));
    
    // Calculate GLCM features
    std::vector<float> glcm_features = calculateGLCM(resized);
    
    Eigen::VectorXf features(128);
    for (int i = 0; i < std::min(128, (int)glcm_features.size()); ++i) {
        features(i) = glcm_features[i];
    }
    
    // Fill remaining features with zeros if needed
    for (int i = glcm_features.size(); i < 128; ++i) {
        features(i) = 0.0f;
    }
    
    return features;
}

Eigen::VectorXf FeatureExtractor::extractNoiseFeatures(const cv::Mat& image) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }
    
    std::vector<float> noise_metrics = calculateNoiseMetrics(gray);
    
    Eigen::VectorXf features(128);
    for (int i = 0; i < std::min(128, (int)noise_metrics.size()); ++i) {
        features(i) = noise_metrics[i];
    }
    
    // Fill remaining features with zeros if needed
    for (int i = noise_metrics.size(); i < 128; ++i) {
        features(i) = 0.0f;
    }
    
    return features;
}

Eigen::VectorXf FeatureExtractor::extractColorFeatures(const cv::Mat& image) {
    if (image.channels() != 3) {
        // Convert grayscale to BGR
        cv::Mat color_img;
        cv::cvtColor(image, color_img, cv::COLOR_GRAY2BGR);
        return extractColorFeatures(color_img);
    }
    
    Eigen::VectorXf features(64);
    
    // Convert to different color spaces
    cv::Mat hsv, lab, yuv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::cvtColor(image, lab, cv::COLOR_BGR2Lab);
    cv::cvtColor(image, yuv, cv::COLOR_BGR2YUV);
    
    // Extract histograms for each channel
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    
    for (int c = 0; c < 3; ++c) {
        std::vector<float> hist = calculateHistogram(channels[c]);
        for (int i = 0; i < 16; ++i) {
            features(c * 16 + i) = hist[i];
        }
    }
    
    // Color statistics
    cv::Scalar mean, stddev;
    cv::meanStdDev(image, mean, stddev);
    
    features(48) = mean[0] / 255.0f; // B
    features(49) = mean[1] / 255.0f; // G
    features(50) = mean[2] / 255.0f; // R
    features(51) = stddev[0] / 255.0f; // B std
    features(52) = stddev[1] / 255.0f; // G std
    features(53) = stddev[2] / 255.0f; // R std
    
    // Color ratios
    features(54) = (mean[1] + 1) / (mean[0] + 1); // G/B ratio
    features(55) = (mean[2] + 1) / (mean[0] + 1); // R/B ratio
    features(56) = (mean[2] + 1) / (mean[1] + 1); // R/G ratio
    
    // Fill remaining features
    for (int i = 57; i < 64; ++i) {
        features(i) = 0.0f;
    }
    
    return features;
}

cv::Mat FeatureExtractor::preprocessImage(const cv::Mat& image) {
    cv::Mat processed = image.clone();
    
    // Resize to standard size
    cv::resize(processed, processed, cv::Size(INPUT_SIZE, INPUT_SIZE));
    
    // Normalize
    processed.convertTo(processed, CV_32F, 1.0/255.0);
    
    return processed;
}

std::vector<float> FeatureExtractor::calculateHistogram(const cv::Mat& image) {
    std::vector<float> histogram(HISTOGRAM_BINS, 0.0f);
    
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            float pixel_value = image.at<uchar>(y, x);
            int bin = static_cast<int>((pixel_value / 255.0f) * (HISTOGRAM_BINS - 1));
            histogram[bin]++;
        }
    }
    
    // Normalize
    float total = image.rows * image.cols;
    for (auto& val : histogram) {
        val /= total;
    }
    
    return histogram;
}

std::vector<float> FeatureExtractor::calculateGLCM(const cv::Mat& image) {
    // Simplified GLCM calculation
    std::vector<float> features;
    
    // Calculate GLCM for different directions
    for (int angle = 0; angle < 4; ++angle) {
        cv::Mat glcm = cv::Mat::zeros(256, 256, CV_32F);
        
        for (int y = 1; y < image.rows - 1; ++y) {
            for (int x = 1; x < image.cols - 1; ++x) {
                int i = image.at<uchar>(y, x);
                
                int j;
                switch (angle) {
                    case 0: j = image.at<uchar>(y, x + 1); break; // 0 degrees
                    case 1: j = image.at<uchar>(y - 1, x + 1); break; // 45 degrees
                    case 2: j = image.at<uchar>(y - 1, x); break; // 90 degrees
                    case 3: j = image.at<uchar>(y - 1, x - 1); break; // 135 degrees
                }
                
                glcm.at<float>(i, j)++;
            }
        }
        
        // Normalize GLCM
        float sum = cv::sum(glcm)[0];
        if (sum > 0) {
            glcm /= sum;
        }
        
        // Extract GLCM features
        cv::Scalar mean = cv::mean(glcm);
        cv::Mat mean_mat, stddev_mat;
        cv::meanStdDev(glcm, mean_mat, stddev_mat);
        
        features.push_back(mean[0]);
        features.push_back(stddev_mat.at<double>(0, 0));
    }
    
    return features;
}

std::vector<float> FeatureExtractor::calculateNoiseMetrics(const cv::Mat& image) {
    std::vector<float> metrics;
    
    // Apply Gaussian blur to estimate noise
    cv::Mat blurred;
    cv::GaussianBlur(image, blurred, cv::Size(5, 5), 0);
    
    // Calculate noise as difference between original and blurred
    cv::Mat noise;
    cv::absdiff(image, blurred, noise);
    
    // Noise statistics
    cv::Scalar mean, stddev;
    cv::meanStdDev(noise, mean, stddev);
    
    metrics.push_back(mean[0] / 255.0f);
    metrics.push_back(stddev[0] / 255.0f);
    
    // Noise distribution
    std::vector<float> noise_hist = calculateHistogram(noise);
    for (int i = 0; i < 32; ++i) {
        metrics.push_back(noise_hist[i]);
    }
    
    // Laplacian variance (edge detection)
    cv::Mat laplacian;
    cv::Laplacian(image, laplacian, CV_32F);
    cv::Scalar lap_mean, lap_stddev;
    cv::meanStdDev(laplacian, lap_mean, lap_stddev);
    
    metrics.push_back(lap_mean[0]);
    metrics.push_back(lap_stddev[0]);
    
    // Fill remaining metrics
    while (metrics.size() < 128) {
        metrics.push_back(0.0f);
    }
    
    return metrics;
} 