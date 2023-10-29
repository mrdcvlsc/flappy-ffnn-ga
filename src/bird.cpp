#include <SFML/Graphics/RectangleShape.hpp>
#include <chrono>
#include <ctime>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include "bird.hpp"

/////////////////////// Bird ///////////////////////

sf::Vector2f Bird::target_gap = {0.f, 0.f};

std::uniform_int_distribution<size_t> Bird::rand_color(0, 255);

Bird::Bird() :
    sf::RectangleShape({SIZE_N, SIZE_N}),
    neural_net(),
    lifetime(0.f),
    speed(JUMP_SPEED * 0.6f),
    fitness(0.f),
    dead(false)
{
    setOrigin(getSize() * 0.5f);
    setFillColor(sf::Color(rand_color(rand_engine), rand_color(rand_engine), rand_color(rand_engine)));
    setOutlineThickness(3.f);
    setOutlineColor(sf::Color::Black);
    reset();
}

void Bird::jump()
{
    speed = JUMP_SPEED;
}

void Bird::update(float dt)
{
    if (!dead) {
        lifetime += dt;

        if (getPosition().y < Bird::SIZE_N * 0.5f) {
            speed = GRAVITY * 0.04f;
        }

        /// distance fall influenced by gravity.
        float distance = (speed * dt) + (0.5f * GRAVITY * dt * dt);

        /// constant acceleration.
        speed += (GRAVITY * dt);

        move(0.f, distance);
    }
}

void Bird::reset()
{
    setPosition(START_X, START_Y);
    lifetime = 0.f;
    speed = JUMP_SPEED * 0.6f;
    dead = false;
}

void Bird::apply_random_mutation()
{
    neural_net.mutate();
}

void Bird::become_offspring(Bird const &b1, Bird const &b2)
{
    sf::Color new_color(b1.getFillColor().r, b2.getFillColor().g, (b1.getFillColor().b + b2.getFillColor().b) % 256);

    setFillColor(new_color);
    neural_net.combine(b1.neural_net, b2.neural_net);
}

/////////////////////// Birds ///////////////////////

Birds::Birds() : collection(), population(Birds::MAX_POPULATION)
{
    for (size_t i = 0; i < MAX_POPULATION; ++i) {
        collection.push_back(Bird());
    }

    reset();
}

void Birds::reset()
{
    for (auto &bird: collection) {
        bird.reset();
    }

    population = MAX_POPULATION;
}

void Birds::update(float dt)
{
    for (auto &bird: collection) {
        bird.update(dt);
    }
}

void Birds::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto &bird: collection) {
        if (!bird.dead) {
            target.draw(bird, states);
        }
    }
}