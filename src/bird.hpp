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
    static constexpr float JUMP_SPEED = -260.f;

    /// Gravitation acceleration.
    static constexpr float GRAVITY = 570.f;

    float time_lived;
    float speed;

    Bird() : sf::RectangleShape({SIZE, SIZE}), time_lived(0.f), speed(JUMP_SPEED * 0.75f) {
        setOrigin(getSize() * 0.5f);
        setFillColor(sf::Color::Red);
        setOutlineThickness(3.f);
        setOutlineColor(sf::Color::Black);
        setPosition(START_X, START_Y);
    }

    void jump() {
        speed = JUMP_SPEED;
    }

    bool is_alive() const {
        if (getPosition().y > static_cast<float>(WINDOW_HEIGHT)) {
            std::cerr << "Bird is dead\n";
            return false;
        }
        return true;
    }

    void update(float dt) {
        if (is_alive()) {
            time_lived += dt;

            if (getPosition().y < Bird::SIZE * 0.5f) {
                speed = GRAVITY * 0.04f;
            }

            /// distance fall influenced by gravity.
            float distance = (speed * dt) + (0.5f * GRAVITY * dt * dt);

            /// constant acceleration.
            speed += (GRAVITY * dt);

            move(0.f, distance);
        }
    }
};

#endif