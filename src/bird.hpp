#ifndef MRDCVLSC_BIRD_HPP
#define MRDCVLSC_BIRD_HPP

#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include <SFML/System/Vector2.hpp>
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

    /// The current `Pipe` gap that the bird needs to jump over.
    static sf::Vector2f target_gap;

    static std::uniform_int_distribution<size_t> rand_color;

    FFNN  neural_net;
    float time_lived;
    float speed;
    float fitness;
    bool  dead;

    Bird();

    void jump();
    void update(float dt);
    void reset();
    void apply_random_mutation();
    void become_offspring(Bird const &b1, Bird const &b2);
};

struct Birds : public sf::Drawable {
    static constexpr size_t INITIAL_POPULATION = 512;

    std::vector<Bird> collection;
    size_t            population;

    Birds();

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    /// \brief resets the value of the members; `time_lived`, `speed`, `dead` and "starting position" of all the `Bird`
    /// in the `Birds` collection to their initial values, then sets the `population` member of the `Birds` class to the
    /// `INITAIL_POPULATION`.
    void reset();

    void update(float dt);
};

#endif