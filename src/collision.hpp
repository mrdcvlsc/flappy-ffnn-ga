#ifndef MRDCVLSC_COLLISION_HPP
#define MRDCVLSC_COLLISION_HPP

#include "bird.hpp"
#include "pipe.hpp"

size_t kill_bird_on_collision(Bird &bird, Pipes const &pipes);
size_t kill_birds_on_collision(Birds &birds, Pipes const &pipes);

#endif