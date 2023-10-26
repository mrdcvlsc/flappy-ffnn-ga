#ifndef MRDCVLSC_GENETIC_ALGORITHM_HPP
#define MRDCVLSC_GENETIC_ALGORITHM_HPP

#include <vector>
#include <cmath>

#include "config.hpp"
#include "bird.hpp"
#include "pipe.hpp"

struct GeneticAlgorithm {
    static constexpr float MUTATION_CUT_OFF = 0.2f;
    static constexpr float MUTATION_KEEP_BAD = 0.50f;

    /// \brief feed inputs to the neural network of each bird, will cause each bird to jump or not jump.
    void get_inputs(Birds &birds, Pipes const &pipes);

    /// \brief calculate the current fitness of each birds in the `Birds` collection and sort them from greatest to
    /// least fitness.
    void rank_fitness(Birds &birds);

    void apply_mutations(Birds &birds);
};

#endif