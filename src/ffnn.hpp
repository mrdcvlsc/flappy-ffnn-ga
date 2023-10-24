#ifndef MRDCVLSC_FFNN_HPP
#define MRDCVLSC_FFNN_HPP

#include <iostream>
#include <cinttypes>
#include <chrono>
#include <random>
#include <stddef.h>
#include <cmath>

#include <eigen3/Eigen/Core>

/// \brief Feed-Forward Neural Network (2x5x1).
struct FFNN {
    static constexpr size_t INPUTS = 2;
    static constexpr size_t HIDDEN = 5;
    static constexpr size_t OUTPUT = 1;

    Eigen::Matrix<float, INPUTS, 1>      input_layer;
    Eigen::Matrix<float, HIDDEN, INPUTS> w1;

    Eigen::Matrix<float, HIDDEN, 1>      hidden_layer;
    Eigen::Matrix<float, OUTPUT, HIDDEN> w2;

    Eigen::Matrix<float, OUTPUT, 1> output_layer;

    FFNN();

    void randomize_network();

    float sigmoid(float x);

    float feedforward();

    void update_inputs(float bird_pipe_distance, float bird_gap_distance);
};

#endif