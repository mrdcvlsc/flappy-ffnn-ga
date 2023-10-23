#include "bird.hpp"

/////////////////////// Bird ///////////////////////

Bird::Bird() : sf::RectangleShape({SIZE, SIZE}), time_lived(0.f), speed(JUMP_SPEED * 0.6f), dead(false), neural_net() {
    setOrigin(getSize() * 0.5f);
    setFillColor(sf::Color::Red);
    setOutlineThickness(3.f);
    setOutlineColor(sf::Color::Black);
    setPosition(START_X, START_Y);
}

void Bird::jump() {
    speed = JUMP_SPEED;
}

void Bird::update(float dt) {
    if (!dead) {
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

/////////////////////// Birds ///////////////////////

Birds::Birds()
    : birds(), population(Birds::INITIAL_POPULATION),
      engine(std::chrono::system_clock::now().time_since_epoch().count()), rng(0ULL, 100ULL) {
    new_generation();
}

void Birds::new_generation() {
    birds.clear();
    for (size_t i = 0; i < INITIAL_POPULATION; ++i) {
        birds.push_back(Bird());
    }
    population = INITIAL_POPULATION;
}

void Birds::update(float dt) {
    for (auto &bird: birds) {
        size_t chance = rng(engine);

        if (chance > 80ULL) {
            bird.jump();
        }

        bird.update(dt);
    }
}

void Birds::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &bird: birds) {
        if (!bird.dead) {
            target.draw(bird, states);
        }
    }
}
