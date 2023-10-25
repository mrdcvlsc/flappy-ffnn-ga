#include <chrono>
#include <ctime>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include "bird.hpp"

/////////////////////// Bird ///////////////////////

sf::Vector2f Bird::target_gap = {0.f, 0.f};

std::mt19937 Bird::color_engine(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<unsigned char> Bird::color_rng(0, 255);

Bird::Bird()
    : sf::RectangleShape({SIZE, SIZE}), time_lived(0.f), speed(JUMP_SPEED * 0.6f), fitness(0.f), dead(false),
      neural_net() {
    setOrigin(getSize() * 0.5f);
    setFillColor(sf::Color(color_rng(color_engine), color_rng(color_engine), color_rng(color_engine)));
    setOutlineThickness(3.f);
    setOutlineColor(sf::Color::Black);
    reset();
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

void Bird::reset() {
    time_lived = 0.f;
    speed = JUMP_SPEED * 0.6f;
    setPosition(START_X, START_Y);
    dead = false;
}

void Bird::apply_random_mutation() {
    neural_net.mutate();
}

void Bird::become_offspring(Bird const &parentA, Bird const &parentB) {
    sf::Color new_color(
      parentA.getFillColor().r | (parentB.getFillColor().r >> 4),
      parentB.getFillColor().g | (parentA.getFillColor().g >> 4),
      parentA.getFillColor().b | (parentB.getFillColor().b << 4)
    );

    setFillColor(new_color);
    neural_net.combine(parentA.neural_net, parentB.neural_net);
}

/////////////////////// Birds ///////////////////////

Birds::Birds() : collection(), population(Birds::INITIAL_POPULATION) {
    for (size_t i = 0; i < INITIAL_POPULATION; ++i) {
        collection.push_back(Bird());
    }

    reset();
}

void Birds::reset() {
    for (auto &bird: collection) {
        bird.reset();
    }

    population = INITIAL_POPULATION;
}

void Birds::update(float dt) {
    for (auto &bird: collection) {
        bird.update(dt);
    }
}

void Birds::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &bird: collection) {
        if (!bird.dead) {
            target.draw(bird, states);

            // a note to self : for debugging purposes / remove later
            sf::CircleShape test(5.f, 3);
            test.setFillColor(sf::Color::Yellow);
            test.setPosition(Bird::target_gap);
            target.draw(test, states);
        }
    }
}