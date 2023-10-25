#include "bird.hpp"
#include "config.hpp"
#include "ffnn.hpp"
#include "genetic_algo.hpp"
#include <algorithm>
#include <iterator>

void GeneticAlgorithm::get_inputs(Birds &birds, Pipes const &pipes) {
    // calculate front/last pipe gap position.
    auto top_pipe1 = pipes.pairs[pipes.front_pipe].top;
    auto top_pipe2 = pipes.pairs[(pipes.front_pipe + 1) % Pipes::COUNT].top;

    Pipe nearest_pipe = top_pipe2;

    if (top_pipe1.getPosition().x + Pipe::WIDTH >= Bird::START_X - Bird::SIZE / 2.f) {
        nearest_pipe = top_pipe1;
    }

    for (auto &bird: birds.collection) {
        // calculate the nearest pipe
        Bird::target_gap =
          sf::Vector2f(nearest_pipe.getPosition().x + Pipe::WIDTH, PipePair::GAP / 2.f + nearest_pipe.getSize().y);

        // normalize the input 1 of the neural network
        float input1_range = (Pipes::START_X + Pipe::WIDTH) - (Bird::START_X - Bird::SIZE / 2.f);
        float input1 = (Bird::target_gap.x - Bird::START_X) - Bird::SIZE / 2.f;
        float input1_normalized = input1 / input1_range;

        // normalize the input 2 of the neural network
        float bird_position_y_range = (WINDOW_HEIGHT - Bird::SIZE / 2.f) - (Bird::SIZE / 2.f);
        float input2_normalized =
          (Bird::target_gap.y - bird.getPosition().y + bird_position_y_range) / bird_position_y_range * 2.f;

        // feed the normalized input to the network and apply feedforward.
        bird.neural_net.update_inputs(std::abs(input1_normalized), input2_normalized);
        float output = bird.neural_net.feedforward();

        // jump if the output of a network is above a certain threshold.
        if (output >= 0.5f) {
            bird.jump();
        }
    }
}

void GeneticAlgorithm::rank_fitness(Birds &birds) {
    // calculate fitness for each bird.
    for (auto &bird: birds.collection) {
        float base_gap_close_fitness = 40.f * bird.neural_net.input_layer(1, 0);
        bird.fitness = base_gap_close_fitness + bird.time_lived * 20.f;
    }

    // sort birds from greatest to least fitness.
    std::cout << "unsorted fitness :\n";
    for (auto &bird: birds.collection) {
        std::cout << "  bird.fitness = " << bird.fitness << '\n';
    }
    std::cout << "\n";

    std::sort(birds.collection.begin(), birds.collection.end(), [](Bird &a, Bird &b) { return a.fitness > b.fitness; });

    std::cout << "sorted fitness :\n";
    for (auto &bird: birds.collection) {
        std::cout << "  bird.fitness = " << bird.fitness << '\n';
    }
    std::cout << "\n\n";
}

void GeneticAlgorithm::apply_mutations(Birds &birds) {
    // apply random mutations to the bad birds.
    constexpr size_t fit_size = static_cast<size_t>(static_cast<float>(Birds::INITIAL_POPULATION) * MUTATION_CUT_OFF);
    for (size_t i = fit_size; i < birds.collection.size(); ++i) {
        birds.collection[i].apply_random_mutation();
    }

    // randomly select mutated bad birds to keep.
    std::cout << "sorted bad bird fitness :\n";
    for (auto &bird: birds.collection) {
        std::cout << "  bird.fitness = " << bird.fitness << '\n';
    }
    std::cout << "\n";

    std::shuffle(birds.collection.begin() + fit_size, birds.collection.end(), FFNN::engine);

    std::cout << "shuffled bad bird fitness :\n";
    for (auto &bird: birds.collection) {
        std::cout << "  bird.fitness = " << bird.fitness << '\n';
    }
    std::cout << "\n";

    // breed the good birds to fill the missing population.
    constexpr size_t unfit_size = (Birds::INITIAL_POPULATION - fit_size) * MUTATION_KEEP_BAD;
    constexpr size_t spliced_size = fit_size + unfit_size;
    constexpr size_t offspring_size = Birds::INITIAL_POPULATION - spliced_size;

    size_t curr_offspring_born = 0;

    for (size_t i = 0; i < fit_size && curr_offspring_born < offspring_size; ++i) {
        for (size_t j = i + 1; j < fit_size && curr_offspring_born < offspring_size; ++j) {
            birds.collection[spliced_size - 1 + curr_offspring_born].become_offspring(
              birds.collection[i], birds.collection[j]
            );
            curr_offspring_born++;
        }
    }
}