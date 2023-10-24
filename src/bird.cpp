#include <ctime>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include "bird.hpp"

/////////////////////// Bird ///////////////////////

Bird::Bird()
    : sf::RectangleShape({SIZE, SIZE}), time_lived(0.f), speed(JUMP_SPEED * 0.6f), fitness(0.f), dead(false),
      neural_net() {
    setOrigin(getSize() * 0.5f);
    setFillColor(sf::Color::Red);
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

/////////////////////// Birds ///////////////////////

Birds::Birds()
    : birds(), population(Birds::INITIAL_POPULATION),
      engine(std::chrono::system_clock::now().time_since_epoch().count()), rng(0ULL, 100ULL) {

    for (size_t i = 0; i < INITIAL_POPULATION; ++i) {
        birds.push_back(Bird());
    }

    reset();
}

void Birds::reset() {
    std::cout << "unsorted fitness :\n";
    for (auto &bird: birds) {
        std::cout << "  bird.fitness = " << bird.fitness << '\n';
    }
    std::cout << "\n";

    std::sort(birds.begin(), birds.end(), [](Bird &a, Bird &b) { return a.fitness > b.fitness; });

    std::cout << "sorted fitness :\n";
    for (auto &bird: birds) {
        std::cout << "  bird.fitness = " << bird.fitness << '\n';
        bird.reset();
    }
    std::cout << "\n\n";

    population = INITIAL_POPULATION;
}

void Birds::update(float dt) {
    for (auto &bird: birds) {
        bird.update(dt);
    }
}

void Birds::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &bird: birds) {
        if (!bird.dead) {
            target.draw(bird, states);

            // a note to self : for debugging purposes / remove later
            sf::CircleShape test(5.f, 3);
            test.setFillColor(sf::Color::Yellow);
            test.setPosition(bird.last_pipe_gap);
            target.draw(test, states);
        }
    }
}
