#include <ios>
#include <fstream>
#include <array>

#include "ffnn.hpp"

std::uniform_real_distribution<float> FFNN::random_chance(0.f, 1.f);
std::uniform_real_distribution<float> FFNN::random_weight(-1.f, 1.f);

FFNN::FFNN() : input_layer(), w1(), hidden_layer(), w2(), output_layer()
{
    randomize_network();
}

void FFNN::randomize_network()
{
    for (size_t j = 0; j < INPUTS; ++j) {
        for (size_t i = 0; i < HIDDEN; ++i) {
            w1(i, j) = random_weight(rand_engine);
        }
    }

    for (size_t j = 0; j < HIDDEN; ++j) {
        for (size_t i = 0; i < OUTPUT; ++i) {
            w2(i, j) = random_weight(rand_engine);
        }
    }
}

float FFNN::sigmoid(float x)
{
    return 1.f / (1.f + std::exp(-x));
}

float FFNN::feedforward()
{
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

void FFNN::update_inputs(float pipe_x_distance, float gap_y_distance)
{
    input_layer(0, 0) = pipe_x_distance;
    input_layer(1, 0) = gap_y_distance;
}

void FFNN::mutate()
{
    // mutate weight 1
    for (size_t j = 0; j < INPUTS; ++j) {
        for (size_t i = 0; i < HIDDEN; ++i) {
            float chance = random_chance(rand_engine);
            if (chance <= MUTATION_CHANCE_THRESHOLD) {
                w1(i, j) = random_weight(rand_engine);
            }
        }
    }

    for (size_t j = 0; j < HIDDEN; ++j) {
        for (size_t i = 0; i < OUTPUT; ++i) {
            float chance = random_chance(rand_engine);
            if (chance <= MUTATION_CHANCE_THRESHOLD) {
                w2(i, j) = random_weight(rand_engine);
            }
        }
    }
}

void FFNN::combine(FFNN const &nn1, FFNN const &nn2)
{
    // mutate weight 1
    for (size_t j = 0; j < INPUTS; ++j) {
        for (size_t i = 0; i < HIDDEN; ++i) {
            float chance = random_chance(rand_engine);
            if (chance <= WEIGHT_SELECTION_CHANCE_THRESHOLD) {
                w1(i, j) = nn1.w1(i, j);
            } else {
                w1(i, j) = nn2.w1(i, j);
            }
        }
    }

    for (size_t j = 0; j < HIDDEN; ++j) {
        for (size_t i = 0; i < OUTPUT; ++i) {
            float chance = random_chance(rand_engine);
            if (chance <= WEIGHT_SELECTION_CHANCE_THRESHOLD) {
                w2(i, j) = nn1.w2(i, j);
            } else {
                w2(i, j) = nn2.w2(i, j);
            }
        }
    }
}

bool FFNN::save_network(std::string const &output_filename)
{
    std::ofstream nn_file(output_filename, std::ios::binary | std::ios::trunc);

    nn_file.write(reinterpret_cast<char *>(w1.data()), w1.size() * sizeof(float));
    nn_file.write(reinterpret_cast<char *>(w2.data()), w2.size() * sizeof(float));
    nn_file.close();

    std::cerr << "------------ saved ------------\n";
    std::cerr << "w1 = \n\n" << w1 << "\n\nw2 = \n\n" << w2 << "\n\n";

    return nn_file.good();
}

bool FFNN::load_network(std::string const &filename)
{
    std::ifstream nn_file(filename, std::ios::binary);

    if (!nn_file.good()) {
        return false;
    }

    nn_file.read(reinterpret_cast<char *>(w1.data()), w1.size() * sizeof(float));
    nn_file.read(reinterpret_cast<char *>(w2.data()), w2.size() * sizeof(float));
    nn_file.close();

    std::cerr << "------------ loaded ------------\n";
    std::cerr << "w1 = \n\n" << w1 << "\n\nw2 = \n\n" << w2 << "\n\n";

    return nn_file.good();
}
