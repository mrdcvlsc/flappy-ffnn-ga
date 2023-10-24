#include "config.hpp"
#include "genetic_algorithm.hpp"

void GeneticAlgorithm::get_inputs(Birds &birds, Pipes const &pipes) {
    // calculate front/last pipe gap position.
    auto top_pipe1 = pipes.pairs[pipes.front_pipe].top;
    auto top_pipe2 = pipes.pairs[(pipes.front_pipe + 1) % Pipes::COUNT].top;

    Pipe nearest_pipe = top_pipe2;

    if (top_pipe1.getPosition().x + Pipe::WIDTH >= Bird::START_X - Bird::SIZE / 2.f) {
        nearest_pipe = top_pipe1;
    }

    for (auto &bird: birds.birds) {
        // calculate the nearest pipe
        bird.last_pipe_gap =
          sf::Vector2f(nearest_pipe.getPosition().x + Pipe::WIDTH, PipePair::GAP / 2.f + nearest_pipe.getSize().y);

        // normalize the input 1 of the neural network
        float input1_range = (Pipes::START_X + Pipe::WIDTH) - (Bird::START_X - Bird::SIZE / 2.f);
        float input1 = (bird.last_pipe_gap.x - Bird::START_X) - Bird::SIZE / 2.f;
        float input1_normalized = input1 / input1_range;

        // normalize the input 2 of the neural network
        float bird_position_y_range = (WINDOW_HEIGHT - Bird::SIZE / 2.f) - (Bird::SIZE / 2.f);
        float input2_normalized = 1.f - (std::abs(bird.last_pipe_gap.y - bird.getPosition().y) / bird_position_y_range);

        // feed the normalized input to the network and apply feedforward.
        bird.neural_net.update_inputs(std::abs(input1_normalized), input2_normalized);
        float output = bird.neural_net.feedforward();

        // jump if the output of a network is above a certain threshold.
        if (output >= 0.5f) {
            bird.jump();
        }
    }
}

void GeneticAlgorithm::calculate_fitness(Birds &birds) {
    for (auto &bird: birds.birds) {
        float base_gap_close_fitness = 40.f * bird.neural_net.input_layer(1, 0);
        bird.fitness = base_gap_close_fitness + bird.time_lived * 20.f;
    }
}