#include <algorithm>
#include <iterator>

#include "genetic_algo.hpp"

void GeneticAlgorithm::get_inputs(Birds &birds, Pipes const &pipes)
{
    // calculate front/last pipe gap position.
    auto top_pipe1 = pipes.pairs[pipes.front_idx].top;
    auto top_pipe2 = pipes.pairs[(pipes.front_idx + 1) % Pipes::COUNT].top;

    Pipe nearest_pipe = top_pipe2;

    if (top_pipe1.getPosition().x + Pipe::WIDTH >= Bird::START_X - Bird::SIZE_N / 2.f) {
        nearest_pipe = top_pipe1;
    }

    // calculate the nearest pipe
    Bird::target_gap =
      sf::Vector2f(nearest_pipe.getPosition().x + Pipe::WIDTH, PipePair::GAP / 2.f + nearest_pipe.getSize().y);

    for (auto &bird: birds.collection) {
        // normalize the input 1 of the neural network
        float max_bird_to_pipe_x_distance = (Pipes::START_X + Pipe::WIDTH) - (Bird::START_X - Bird::SIZE_N / 2.f);
        float input1 = (Bird::target_gap.x - Bird::START_X) - Bird::SIZE_N / 2.f;
        float input1_normalized = input1 / max_bird_to_pipe_x_distance;

        // normalize the input 2 of the neural network
        float max_bird_to_gap_y_distance = (WINDOW_HEIGHT - Bird::SIZE_N / 2.f) - (Bird::SIZE_N / 2.f);
        float input2_normalized =
          (Bird::target_gap.y - bird.getPosition().y + max_bird_to_gap_y_distance) / max_bird_to_gap_y_distance * 2.f;

        // feed the normalized inputs to the network (feedforward).
        bird.neural_net.update_inputs(std::abs(input1_normalized), input2_normalized);
        float output = bird.neural_net.feedforward();

        // jump if the output of a network is above a certain threshold.
        if (output >= 0.5f) {
            bird.jump();
        }
    }
}

void GeneticAlgorithm::rank_fitness(Birds &birds)
{
    // calculate fitness for each bird.
    for (auto &bird: birds.collection) {
        float bird_to_gap_y_distance = bird.neural_net.input_layer(1, 0);
        bird.fitness = 40.f * bird_to_gap_y_distance + bird.lifetime * 20.f;
    }

    std::sort(birds.collection.begin(), birds.collection.end(), [](Bird &a, Bird &b) { return a.fitness > b.fitness; });
}

void GeneticAlgorithm::apply_mutations(Birds &birds)
{
    // apply random mutations to the bad birds.
    constexpr size_t fit_n = static_cast<size_t>(static_cast<float>(Birds::MAX_POPULATION) * FITTEST_TO_KEEP);
    for (size_t i = fit_n; i < birds.collection.size(); ++i) {
        birds.collection[i].apply_random_mutation();
    }

    std::shuffle(birds.collection.begin() + fit_n, birds.collection.end(), rand_engine);

    // breed the good birds to fill the missing population.
    constexpr size_t unfit_n = (Birds::MAX_POPULATION - fit_n) * UNFIT_TO_KEEP;
    constexpr size_t splice_n = fit_n + unfit_n;
    constexpr size_t offspring_n = Birds::MAX_POPULATION - splice_n;

    size_t curr_offsprings = 0;

    for (size_t i = 0; i < fit_n && curr_offsprings < offspring_n; ++i) {
        for (size_t j = i + 1; j < fit_n && curr_offsprings < offspring_n; ++j) {
            birds.collection[splice_n - 1 + curr_offsprings].become_offspring(birds.collection[i], birds.collection[j]);
            curr_offsprings++;
        }
    }
}