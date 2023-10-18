#ifndef MRDCVLSC_PIPE_HPP
#define MRDCVLSC_PIPE_HPP

#include "config.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>

enum class pipe_type
{
    top,
    bottom
};

struct Pipe : public sf::RectangleShape {
    /// Default wideness of a pipe in pixel.
    static constexpr float WIDTH = 30.f;

    /// Pipe movement speed in pixel per second.
    static constexpr float SPEED = 40.f;

    Pipe(float height, float x_pos, pipe_type position) : sf::RectangleShape({WIDTH, height}) {
        setFillColor(sf::Color::Green);
        setOutlineColor(sf::Color::Black);
        setOutlineThickness(3.f);

        if (position == pipe_type::bottom) {
            setPosition({x_pos, WINDOW_HEIGHT - height});
        } else {
            setPosition({x_pos, 0.f});
        }
    }

    /// \param tick elapsed per frame.
    void update(float tick) {
        move({-tick * SPEED, 0});
    }
};

#endif