#ifndef MRDCVLSC_BIRD_HPP
#define MRDCVLSC_BIRD_HPP

#include <iostream>

#include <SFML/Graphics/RectangleShape.hpp>

#include "config.hpp"

struct Bird : public sf::RectangleShape {
    /// Default bird side dimension length (SIZE x SIZE).
    static constexpr float SIZE = 20.f;

    /// Starting x position of the bird in the 2D world.
    static constexpr float START_X = static_cast<float>(WINDOW_WIDTH) / 5.f;

    /// Starting y position of the bird in the 2D world.
    static constexpr float START_Y = static_cast<float>(WINDOW_HEIGHT) * 0.5f;

    /// Bird initial vertical speed.
    static constexpr float JUMP_SPEED = -280.f;

    /// Gravitation acceleration.
    static constexpr float GRAVITY = 700.f;

    float time_lived;
    float speed;

    Bird();

    bool is_alive() const;
    void jump();
    void update(float dt);
};

#endif