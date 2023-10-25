#ifndef MRDCVLSC_COLLISION_HPP
#define MRDCVLSC_COLLISION_HPP

#include "bird.hpp"
#include "pipe.hpp"

/// \brief kill a bird when it collides to a pipe.
/// \returns 0 if the bird is not dead or is already dead.
/// \returns 1 if the bird is alive at first then died.
size_t bird_collision(Bird &bird, Pipes const &pipes);

/// \brief kill all birds that collides with a pipe.
/// \returns the total number of deaths in the current `Birds` class collection.
size_t birds_collisions(Birds &birds, Pipes const &pipes);

#endif