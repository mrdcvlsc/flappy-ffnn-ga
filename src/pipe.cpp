#include "pipe.hpp"

////////////////////////// Pipe //////////////////////////

Pipe::Pipe() {
}

Pipe::Pipe(float height, float x_pos, pipe_type position) : sf::RectangleShape({WIDTH, height}), position(position) {
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
void Pipe::update(float tick) {
    move({-tick * SPEED, 0});
}

////////////////////////// PipePair //////////////////////////

PipePair::PipePair(float pos_x, float top_height) {
    top = Pipe(top_height, pos_x, pipe_type::top);
    btm = Pipe(static_cast<float>(WINDOW_HEIGHT) - top_height - GAP, pos_x, pipe_type::bottom);
}

PipePair::PipePair() : PipePair(0.f, 0.f) {
}

void PipePair::update(float tick) {
    top.update(tick);
    btm.update(tick);
}

void PipePair::set_pos(float pos_x) {
    top.setPosition(pos_x, top.getPosition().y);
    btm.setPosition(pos_x, btm.getPosition().y);
}

const sf::Vector2f &PipePair::getPosition() const {
    return top.getPosition();
}

void PipePair::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(top, states);
    target.draw(btm, states);
}

////////////////////////// Pipes //////////////////////////

Pipes::Pipes()
    : engine(std::chrono::system_clock::now().time_since_epoch().count()),
      rng(PipePair::MIN_HEIGHT, PipePair::MAX_HEIGHT), front_pipe(0ULL) {
    for (size_t i = 0; i < COUNT; ++i) {
        pairs[i] = PipePair(START_X + i * DISTANCE, rng(engine));
    }
}

void Pipes::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (PipePair const &pipe_pair: pairs) {
        target.draw(pipe_pair, states);
    }
}

/// \param tick elapsed per frame.
void Pipes::update(float tick) {
    size_t new_front_pipe = front_pipe;
    for (size_t i = 0; i < COUNT; ++i) {
        size_t index = (front_pipe + i) % COUNT;

        if (pairs[index].getPosition().x + Pipe::WIDTH < 0.f) {
            size_t back_index = (front_pipe + COUNT - 1) % COUNT;
            pairs[index].set_pos(pairs[back_index].getPosition().x + DISTANCE);

            // generate new heights for the pipe that was moved to the back.
            float top_height = rng(engine);
            pairs[index].top.setSize({Pipe::WIDTH, top_height});
            pairs[index].btm.setSize({Pipe::WIDTH, static_cast<float>(WINDOW_HEIGHT) - top_height - PipePair::GAP});
            pairs[index].btm.setPosition(pairs[index].btm.getPosition().x, top_height + PipePair::GAP);

            new_front_pipe = (new_front_pipe + 1) % COUNT;
        }

        pairs[index].update(tick);
    }
    front_pipe = new_front_pipe;
}

void Pipes::new_generation() {
    front_pipe = 0ULL;
    for (size_t i = 0; i < COUNT; ++i) {
        pairs[i] = PipePair(START_X + i * DISTANCE, rng(engine));
    }
}