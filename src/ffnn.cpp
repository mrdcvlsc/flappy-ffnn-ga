#include "ffnn.hpp"

FFNN::FFNN() : input_layer(), w1(), hidden_layer(), w2(), output_layer() {
    randomize_network();
}

void FFNN::randomize_network() {
    std::mt19937                          engine(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> rng(-0.5f, 0.5f);

    for (size_t j = 0; j < INPUTS; ++j) {
        for (size_t i = 0; i < HIDDEN; ++i) {
            w1(i, j) = rng(engine);
        }
    }

    for (size_t j = 0; j < HIDDEN; ++j) {
        for (size_t i = 0; i < OUTPUT; ++i) {
            w2(i, j) = rng(engine);
        }
    }
}

float FFNN::sigmoid(float x) {
    return 1.f / (1.f + std::exp(-x));
}

float FFNN::feedforward() {
    hidden_layer.noalias() = w1 * input_layer;

    // layer activation - apply sigmoid to the output of the hidden layer.
    for (size_t i = 0; i < HIDDEN; ++i) {
        hidden_layer(i, 0) = sigmoid(hidden_layer(i, 0));
    }

    output_layer.noalias() = w2 * hidden_layer;

    // activate output layer.
    output_layer(0, 0) = sigmoid(output_layer(0, 0));

    return output_layer(0, 0);
}

void FFNN::update_inputs(float bird_pipe_distance, float bird_gap_distance) {
    input_layer(0, 0) = bird_pipe_distance;
    input_layer(1, 0) = bird_gap_distance;
}