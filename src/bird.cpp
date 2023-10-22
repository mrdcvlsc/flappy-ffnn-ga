#include "bird.hpp"

Bird::Bird() : sf::RectangleShape({SIZE, SIZE}), time_lived(0.f), speed(JUMP_SPEED * 0.75f) {
    setOrigin(getSize() * 0.5f);
    setFillColor(sf::Color::Red);
    setOutlineThickness(3.f);
    setOutlineColor(sf::Color::Black);
    setPosition(START_X, START_Y);
}

bool Bird::is_alive() const {
    return getPosition().y < static_cast<float>(WINDOW_HEIGHT);
}

void Bird::jump() {
    speed = JUMP_SPEED;
}

void Bird::update(float dt) {
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