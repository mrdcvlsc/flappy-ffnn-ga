#ifndef MRDCVLSC_BIRD_HPP
#define MRDCVLSC_BIRD_HPP

#include "config.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

struct Bird : public sf::RectangleShape {
    /// Default bird side dimension length (SIZE x SIZE).
    static constexpr float SIZE = 20.f;

    /// Starting x position of the bird in the 2D world.
    static constexpr float START_X_POS = static_cast<float>(WINDOW_WIDTH) / 5.f;

    /// Starting y position of the bird in the 2D world.
    static constexpr float START_Y_POS = static_cast<float>(WINDOW_HEIGHT) / 2.f;

    Bird() : sf::RectangleShape({SIZE, SIZE}) {
        setOrigin(getSize() / 2.f);
        setFillColor(sf::Color::Red);
        setOutlineThickness(3.f);
        setOutlineColor(sf::Color::Black);
        setPosition(START_X_POS, START_Y_POS);
    }
};

#endif