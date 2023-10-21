#ifndef MRDCVLSC_PIPE_HPP
#define MRDCVLSC_PIPE_HPP

#include <iostream>
#include <array>
#include <chrono>
#include <random>

#include <cstddef>
#include <cstdint>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "bird.hpp"
#include "config.hpp"

template <typename T>
constexpr T mod(T n, T M) {
    return ((n % M) + M) % M;
}

enum class pipe_type
{
    top,
    bottom
};

struct Pipe : public sf::RectangleShape {
    /// Default wideness of a pipe in pixel.
    static constexpr float WIDTH = 30.f;

    /// Pipe movement speed in pixel per second.
    static constexpr float SPEED = 80.f;

    pipe_type position;

    Pipe() {
    }

    Pipe(float height, float x_pos, pipe_type position) : sf::RectangleShape({WIDTH, height}), position(position) {
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

struct PipePair : public sf::Drawable {
    /// Y axis space between two pair of pipes.
    static constexpr float GAP = Bird::SIZE * 6.f;

    /// top pipe's minimum height.
    static constexpr float MIN_HEIGHT = static_cast<float>(WINDOW_HEIGHT) / 12.f;

    /// top pipe's maximum height.
    static constexpr float MAX_HEIGHT = static_cast<float>(WINDOW_HEIGHT) - MIN_HEIGHT - GAP;

    std::mt19937                          engine;
    std::uniform_real_distribution<float> rng;

    Pipe top;
    Pipe btm;

    PipePair(float pos_x)
        : engine(std::chrono::system_clock::now().time_since_epoch().count()), rng(MIN_HEIGHT, MAX_HEIGHT) {
        float top_height = rng(engine);
        top = Pipe(top_height, pos_x, pipe_type::top);
        btm = Pipe(static_cast<float>(WINDOW_HEIGHT) - top_height - GAP, pos_x, pipe_type::bottom);
    }

    PipePair() : PipePair(0.f) {
    }

    void update(float tick) {
        top.update(tick);
        btm.update(tick);
    }

    void new_height() {
        float top_height = rng(engine);
        top.setSize({Pipe::WIDTH, top_height});
        btm.setSize({Pipe::WIDTH, static_cast<float>(WINDOW_HEIGHT) - top_height - GAP});
        btm.setPosition(btm.getPosition().x, top_height + GAP);
    }

    void set_pos(float pos_x) {
        top.setPosition(pos_x, top.getPosition().y);
        btm.setPosition(pos_x, btm.getPosition().y);
    }

    virtual const sf::Vector2f &getPosition() const {
        return top.getPosition();
    }

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(top, states);
        target.draw(btm, states);
    }
};

/// fixed collection of `PipePair` classes.
/// \tparam N - number of pipes to spawn.
template <size_t N>
struct Pipes : public sf::Drawable {
    /// starting position of the first pipe.
    static constexpr float start_x = Bird::START_X_POS + static_cast<float>(WINDOW_WIDTH) * 0.2f;

    /// X axis space between each set of pipe pairs.
    static constexpr float DISTANCE = Pipe::WIDTH + Bird::SIZE * 5.f;

    std::array<PipePair, N> pairs;

    /// Current front pipe index.
    size_t front_pipe;

    Pipes() : front_pipe(0) {
        for (size_t i = 0; i < N; ++i) {
            pairs[i] = PipePair(start_x + i * DISTANCE);
        }
    }

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        for (PipePair const &pipe_pair: pairs) {
            target.draw(pipe_pair, states);
        }
    }

    /// \param tick elapsed per frame.
    void update(float tick) {
        size_t new_front_pipe = front_pipe;
        for (size_t i = 0; i < N; ++i) {
            size_t index = (front_pipe + i) % N;

            if (pairs[index].getPosition().x + Pipe::WIDTH < 0.f) {
                size_t back_index = (front_pipe + N - 1) % N;
                pairs[index].set_pos(pairs[back_index].getPosition().x + DISTANCE);
                pairs[index].new_height();

                new_front_pipe = (new_front_pipe + 1) % N;
            }

            pairs[index].update(tick);
        }
        front_pipe = new_front_pipe;
    }
};

#endif