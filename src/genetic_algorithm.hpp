#ifndef MRDCVLSC_GENETIC_ALGORITHM_HPP
#define MRDCVLSC_GENETIC_ALGORITHM_HPP

#include <vector>
#include <cmath>

#include "bird.hpp"
#include "pipe.hpp"

class GeneticAlgorithm {
    std::vector<Bird> bird_selections;

    public:

    void get_inputs(Birds &birds, Pipes const &pipes);
};

#endif