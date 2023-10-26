#ifndef MRDCVLSC_PIPE_HPP
#define MRDCVLSC_PIPE_HPP

#include <iostream>
#include <array>

#include <cstddef>
#include <cstdint>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "config.hpp"

template <typename T>
constexpr T mod(T n, T M)
{
    return ((n % M) + M) % M;
}

enum class pipe_t
{
    top,
    bottom
};

////////////////////////// Pipe //////////////////////////

struct Pipe : public sf::RectangleShape
{
    /// Default wideness of a pipe in pixel.
    static constexpr float WIDTH = 30.f;

    /// Pipe movement speed in pixel per second.
    static constexpr float SPEED = 80.f;

    pipe_t position;

    Pipe();
    Pipe(float height, float x_pos, pipe_t position);

    /// \param tick elapsed per frame.
    void update(float tick);
};

////////////////////////// PipePair //////////////////////////

struct PipePair : public sf::Drawable
{
    /// Y axis space between two pair of pipes.
    static constexpr float GAP = 110.f;

    /// top pipe's minimum height.
    static constexpr float MIN_HEIGHT = static_cast<float>(WINDOW_HEIGHT) / 40.f;

    /// top pipe's maximum height.
    static constexpr float MAX_HEIGHT = static_cast<float>(WINDOW_HEIGHT) - MIN_HEIGHT - GAP;

    Pipe top;
    Pipe btm;

    PipePair(float pos_x, float top_height);
    PipePair();

    void                update(float tick);
    void                set_pos(float pos_x);
    const sf::Vector2f &getPosition() const;
    virtual void        draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

////////////////////////// Pipes //////////////////////////

/// six collection of `PipePair` classes.
struct Pipes : public sf::Drawable
{
    /// starting position of the first pipe in the 2D world.
    static constexpr float START_X = static_cast<float>(WINDOW_WIDTH) * 0.45f;

    /// X axis space between each set of pipe pairs.
    static constexpr float DISTANCE = Pipe::WIDTH + 100.f;

    static constexpr size_t COUNT = 6;

    std::array<PipePair, COUNT> pairs;

    std::uniform_real_distribution<float> rand_height;

    /// Current front pipe index.
    size_t front_idx;

    Pipes();

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    /// \param tick elapsed per frame.
    void update(float tick);

    void new_generation();
};

#endif