#ifndef MRDCVLSC_COLLISION_HPP
#define MRDCVLSC_COLLISION_HPP

#include "bird.hpp"
#include "pipe.hpp"

/// \returns 0 if the bird is not dead or is already dead.
/// \returns 1 if the bird is alive at first then died.
size_t kill_bird_on_collision(Bird &bird, Pipes const &pipes);

/// \returns the total number of deaths in the current `Birds` class collection.
size_t kill_birds_on_collision(Birds &birds, Pipes const &pipes);

#endif