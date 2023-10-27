#ifndef MRDCVLSC_FFNN_HPP
#define MRDCVLSC_FFNN_HPP

#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

#include <stddef.h>

#include <cinttypes>
#include <cmath>

#include <Eigen/Core>

#include "config.hpp"

/// \brief Feed-Forward Neural Network (2x5x1).
struct FFNN
{
    static constexpr size_t INPUTS = 2;
    static constexpr size_t HIDDEN = 5;
    static constexpr size_t OUTPUT = 1;

    /// \brief 30% chance to mutate a weight.
    static constexpr size_t MUTATION_CHANCE_THRESHOLD = 30;

    /// \brief 50% chance to choose from parentA's weight.
    static constexpr size_t WEIGHT_SELECTION_CHANCE_THRESHOLD = 50;

    /// \brief generates number from 1 to 100.
    static std::uniform_int_distribution<size_t> random_chance;

    /// \brief generates weight from -0.5 to 0.5.
    static std::uniform_real_distribution<float> new_random_weight;

    Eigen::Matrix<float, INPUTS, 1>      input_layer;
    Eigen::Matrix<float, HIDDEN, INPUTS> w1;

    Eigen::Matrix<float, HIDDEN, 1>      hidden_layer;
    Eigen::Matrix<float, OUTPUT, HIDDEN> w2;

    Eigen::Matrix<float, OUTPUT, 1> output_layer;

    FFNN();

    void randomize_network();

    float sigmoid(float x);

    float feedforward();

    void update_inputs(float pipe_x_distance, float gap_y_distance);

    /// \brief randomly mutate the current weights.
    void mutate();

    /// \brief overwrite the old weights with the weights of either network1 or network2 (50/50 random selection).
    void combine(FFNN const &net1, FFNN const &net2);

    bool save_network(std::string const &output_filename);
    bool load_network(std::string const &filename);
};

#endif