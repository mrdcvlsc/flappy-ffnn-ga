#ifndef MRDCVLSC_BIRD_HPP
#define MRDCVLSC_BIRD_HPP

#include <iostream>
#include <chrono>
#include <random>
#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "config.hpp"
#include "ffnn.hpp"

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
    bool  dead;
    FFNN  neural_net;

    Bird();

    void jump();
    void update(float dt);
};

struct Birds : public sf::Drawable {
    static constexpr size_t INITIAL_POPULATION = 60;

    std::vector<Bird>                     birds;
    size_t                                population;
    std::mt19937                          engine;
    std::uniform_int_distribution<size_t> rng;

    Birds();

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void         new_generation();
    void         update(float dt);
};

#endif