#pragma once

#include <Eigen/Dense>
#include <vector>
#include <string>
#include <random>

class NeuralNetwork {
public:
    NeuralNetwork();
    ~NeuralNetwork() = default;

    // Initialize network architecture
    void initialize(const std::vector<int>& layer_sizes);
    
    // Forward pass
    Eigen::VectorXf forward(const Eigen::VectorXf& input);
    
    // Training methods
    void train(const std::vector<Eigen::VectorXf>& inputs, 
               const std::vector<Eigen::VectorXf>& targets,
               float learning_rate = 0.01f,
               int epochs = 100);
    
    // Prediction
    float predict(const Eigen::VectorXf& input);
    
    // Save/load model
    bool saveModel(const std::string& filename);
    bool loadModel(const std::string& filename);
    
    // Set/get parameters
    void setLearningRate(float lr) { learning_rate_ = lr; }
    float getLearningRate() const { return learning_rate_; }

private:
    // Network layers
    std::vector<Eigen::MatrixXf> weights_;
    std::vector<Eigen::VectorXf> biases_;
    std::vector<Eigen::VectorXf> activations_;
    std::vector<Eigen::VectorXf> z_values_;
    
    // Training parameters
    float learning_rate_;
    std::mt19937 rng_;
    
    // Activation functions
    Eigen::VectorXf sigmoid(const Eigen::VectorXf& x);
    Eigen::VectorXf sigmoidDerivative(const Eigen::VectorXf& x);
    Eigen::VectorXf relu(const Eigen::VectorXf& x);
    Eigen::VectorXf reluDerivative(const Eigen::VectorXf& x);
    
    // Loss function
    float crossEntropyLoss(const Eigen::VectorXf& predicted, const Eigen::VectorXf& target);
    Eigen::VectorXf crossEntropyDerivative(const Eigen::VectorXf& predicted, const Eigen::VectorXf& target);
    
    // Backpropagation
    void backward(const Eigen::VectorXf& input, const Eigen::VectorXf& target);
    void updateWeights(float learning_rate);
    
    // Helper methods
    Eigen::VectorXf softmax(const Eigen::VectorXf& x);
    void initializeWeights();
}; 