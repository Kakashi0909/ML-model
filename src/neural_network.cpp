#include "../include/neural_network.h"
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cmath>

NeuralNetwork::NeuralNetwork() : learning_rate_(0.01f) {
    rng_.seed(std::random_device{}());
}

void NeuralNetwork::initialize(const std::vector<int>& layer_sizes) {
    if (layer_sizes.size() < 2) {
        throw std::invalid_argument("At least 2 layers required (input and output)");
    }
    
    weights_.clear();
    biases_.clear();
    
    // Initialize weights and biases for each layer
    for (size_t i = 0; i < layer_sizes.size() - 1; ++i) {
        int input_size = layer_sizes[i];
        int output_size = layer_sizes[i + 1];
        
        // Xavier initialization
        float scale = std::sqrt(2.0f / input_size);
        std::normal_distribution<float> dist(0.0f, scale);
        
        Eigen::MatrixXf weight(output_size, input_size);
        for (int row = 0; row < output_size; ++row) {
            for (int col = 0; col < input_size; ++col) {
                weight(row, col) = dist(rng_);
            }
        }
        weights_.push_back(weight);
        
        Eigen::VectorXf bias(output_size);
        bias.setZero();
        biases_.push_back(bias);
    }
    
    // Initialize activation and z-value storage
    activations_.resize(layer_sizes.size());
    z_values_.resize(layer_sizes.size() - 1);
}

Eigen::VectorXf NeuralNetwork::forward(const Eigen::VectorXf& input) {
    if (weights_.empty()) {
        throw std::runtime_error("Network not initialized");
    }
    
    // Set input as first activation
    activations_[0] = input;
    
    // Forward pass through all layers
    for (size_t i = 0; i < weights_.size(); ++i) {
        // Linear transformation
        z_values_[i] = weights_[i] * activations_[i] + biases_[i];
        
        // Activation function (ReLU for hidden layers, sigmoid for output)
        if (i == weights_.size() - 1) {
            // Output layer - sigmoid
            activations_[i + 1] = sigmoid(z_values_[i]);
        } else {
            // Hidden layers - ReLU
            activations_[i + 1] = relu(z_values_[i]);
        }
    }
    
    return activations_.back();
}

void NeuralNetwork::train(const std::vector<Eigen::VectorXf>& inputs, 
                         const std::vector<Eigen::VectorXf>& targets,
                         float learning_rate, int epochs) {
    if (inputs.size() != targets.size()) {
        throw std::invalid_argument("Input and target sizes must match");
    }
    
    learning_rate_ = learning_rate;
    
    for (int epoch = 0; epoch < epochs; ++epoch) {
        float total_loss = 0.0f;
        
        // Shuffle training data
        std::vector<size_t> indices(inputs.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), rng_);
        
        for (size_t idx : indices) {
            // Forward pass
            Eigen::VectorXf output = forward(inputs[idx]);
            
            // Calculate loss
            float loss = crossEntropyLoss(output, targets[idx]);
            total_loss += loss;
            
            // Backward pass
            backward(inputs[idx], targets[idx]);
            
            // Update weights
            updateWeights(learning_rate_);
        }
        
        // Print progress
        if (epoch % 10 == 0) {
            std::cout << "Epoch " << epoch << ", Average Loss: " 
                      << total_loss / inputs.size() << std::endl;
        }
    }
}

float NeuralNetwork::predict(const Eigen::VectorXf& input) {
    Eigen::VectorXf output = forward(input);
    return output(0); // Return first (and only) output value
}

bool NeuralNetwork::saveModel(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // Save network architecture
    size_t num_layers = weights_.size() + 1;
    file.write(reinterpret_cast<const char*>(&num_layers), sizeof(num_layers));
    
    // Save layer sizes
    std::vector<int> layer_sizes;
    if (!weights_.empty()) {
        layer_sizes.push_back(weights_[0].cols()); // Input size
        for (const auto& weight : weights_) {
            layer_sizes.push_back(weight.rows()); // Output size
        }
    }
    
    size_t layer_sizes_size = layer_sizes.size();
    file.write(reinterpret_cast<const char*>(&layer_sizes_size), sizeof(layer_sizes_size));
    file.write(reinterpret_cast<const char*>(layer_sizes.data()), 
               layer_sizes.size() * sizeof(int));
    
    // Save weights
    for (const auto& weight : weights_) {
        size_t rows = weight.rows();
        size_t cols = weight.cols();
        file.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
        file.write(reinterpret_cast<const char*>(&cols), sizeof(cols));
        file.write(reinterpret_cast<const char*>(weight.data()), 
                   weight.size() * sizeof(float));
    }
    
    // Save biases
    for (const auto& bias : biases_) {
        size_t size = bias.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        file.write(reinterpret_cast<const char*>(bias.data()), 
                   bias.size() * sizeof(float));
    }
    
    return true;
}

bool NeuralNetwork::loadModel(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    // Load network architecture
    size_t num_layers;
    file.read(reinterpret_cast<char*>(&num_layers), sizeof(num_layers));
    
    // Load layer sizes
    size_t layer_sizes_size;
    file.read(reinterpret_cast<char*>(&layer_sizes_size), sizeof(layer_sizes_size));
    std::vector<int> layer_sizes(layer_sizes_size);
    file.read(reinterpret_cast<char*>(layer_sizes.data()), 
              layer_sizes.size() * sizeof(int));
    
    // Initialize network
    initialize(layer_sizes);
    
    // Load weights
    for (auto& weight : weights_) {
        size_t rows, cols;
        file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
        file.read(reinterpret_cast<char*>(&cols), sizeof(cols));
        weight.resize(rows, cols);
        file.read(reinterpret_cast<char*>(weight.data()), 
                  weight.size() * sizeof(float));
    }
    
    // Load biases
    for (auto& bias : biases_) {
        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        bias.resize(size);
        file.read(reinterpret_cast<char*>(bias.data()), 
                  bias.size() * sizeof(float));
    }
    
    return true;
}

Eigen::VectorXf NeuralNetwork::sigmoid(const Eigen::VectorXf& x) {
    return 1.0f / (1.0f + (-x).array().exp());
}

Eigen::VectorXf NeuralNetwork::sigmoidDerivative(const Eigen::VectorXf& x) {
    Eigen::VectorXf sig = sigmoid(x);
    return sig.array() * (1.0f - sig.array());
}

Eigen::VectorXf NeuralNetwork::relu(const Eigen::VectorXf& x) {
    return x.array().max(0.0f);
}

Eigen::VectorXf NeuralNetwork::reluDerivative(const Eigen::VectorXf& x) {
    return (x.array() > 0.0f).cast<float>();
}

float NeuralNetwork::crossEntropyLoss(const Eigen::VectorXf& predicted, const Eigen::VectorXf& target) {
    // Binary cross-entropy loss
    float epsilon = 1e-15f;
    float p = std::max(std::min(predicted(0), 1.0f - epsilon), epsilon);
    float t = target(0);
    return -t * std::log(p) - (1.0f - t) * std::log(1.0f - p);
}

Eigen::VectorXf NeuralNetwork::crossEntropyDerivative(const Eigen::VectorXf& predicted, const Eigen::VectorXf& target) {
    // Derivative of binary cross-entropy loss
    float epsilon = 1e-15f;
    float p = std::max(std::min(predicted(0), 1.0f - epsilon), epsilon);
    float t = target(0);
    Eigen::VectorXf derivative(1);
    derivative(0) = (p - t) / (p * (1.0f - p));
    return derivative;
}

void NeuralNetwork::backward(const Eigen::VectorXf& input, const Eigen::VectorXf& target) {
    // Calculate gradients for output layer
    Eigen::VectorXf output_gradient = crossEntropyDerivative(activations_.back(), target);
    
    // Backpropagate through layers
    for (int i = static_cast<int>(weights_.size()) - 1; i >= 0; --i) {
        // Gradient with respect to weights
        Eigen::MatrixXf weight_gradient = output_gradient * activations_[i].transpose();
        
        // Gradient with respect to bias
        Eigen::VectorXf bias_gradient = output_gradient;
        
        // Gradient with respect to input (for next layer)
        if (i > 0) {
            Eigen::VectorXf input_gradient = weights_[i].transpose() * output_gradient;
            
            // Apply activation derivative
            Eigen::VectorXf activation_derivative = reluDerivative(z_values_[i - 1]);
            output_gradient = input_gradient.array() * activation_derivative.array();
        }
        
        // Store gradients (in a real implementation, you'd accumulate these)
        // For simplicity, we'll update weights directly in updateWeights()
    }
}

void NeuralNetwork::updateWeights(float learning_rate) {
    // Simplified weight update (in a real implementation, you'd use stored gradients)
    // This is a placeholder for the actual gradient descent update
    for (size_t i = 0; i < weights_.size(); ++i) {
        // weights_[i] -= learning_rate * weight_gradients_[i];
        // biases_[i] -= learning_rate * bias_gradients_[i];
    }
}

Eigen::VectorXf NeuralNetwork::softmax(const Eigen::VectorXf& x) {
    Eigen::VectorXf exp_x = x.array().exp();
    return exp_x / exp_x.sum();
} 