#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <memory>

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "bird.hpp"
#include "config.hpp"
#include "pipe.hpp"
#include "gamestats.hpp"
#include "collision.hpp"
#include "genetic_algo.hpp"

int main()
{
    auto game_statistics = std::make_shared<GameStats>();

    sf::RectangleShape game_area({WINDOW_WIDTH, WINDOW_HEIGHT});
    game_area.setFillColor(sf::Color(0, 0, 0, 0));
    game_area.setOutlineColor(sf::Color::Black);
    game_area.setOutlineThickness(2.f);

    sf::View      game_view(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));
    sf::VideoMode video_dimension(WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::RenderWindow window(video_dimension, "Flappy FFNN-GA");
    window.setView(game_view);
    window.setFramerateLimit(GameStats::FRAME_LIMIT);

    sf::Color background(19, 235, 220);

    Birds            birds;
    Pipes            pipes;
    GeneticAlgorithm genetic_algorithm;

    while (window.isOpen()) {
        sf::Time dt = game_statistics->clock_fps.restart();
        game_statistics->update_time_elapsed += dt;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                // cam up
                if (event.key.scancode == sf::Keyboard::Scan::Up) {
                    game_view.move(0.f, -VIEW_MOVE_DISTANCE);
                }

                // cam down
                if (event.key.scancode == sf::Keyboard::Scan::Down) {
                    game_view.move(0.f, VIEW_MOVE_DISTANCE);
                }

                // cam left
                if (event.key.scancode == sf::Keyboard::Scan::Left) {
                    game_view.move(-VIEW_MOVE_DISTANCE, 0.f);
                }

                // cam right
                if (event.key.scancode == sf::Keyboard::Scan::Right) {
                    game_view.move(VIEW_MOVE_DISTANCE, 0.f);
                }

                // zoom in cam
                if (event.key.scancode == sf::Keyboard::Scan::Equal) {
                    game_view.zoom(0.99f);
                }

                // zoom out cam
                if (event.key.scancode == sf::Keyboard::Scan::Hyphen) {
                    game_view.zoom(1.01f);
                }

                // bird jumps
                if (event.key.scancode == sf::Keyboard::Scan::Space) {
                    for (auto &bird: birds.collection) {
                        bird.jump();
                    }
                }

                window.setView(game_view);
            }
        }

        genetic_algorithm.get_inputs(birds, pipes);

        while (game_statistics->update_time_elapsed > GameStats::TIME_PER_FRAME) {
            game_statistics->update_time_elapsed -= GameStats::TIME_PER_FRAME;

            pipes.update(GameStats::TIME_PER_FRAME.asSeconds());
            birds.update(GameStats::TIME_PER_FRAME.asSeconds());

            game_statistics->record_deaths(birds_collisions(birds, pipes));
        }

        window.clear(background);

        window.draw(game_area);
        window.draw(birds);
        window.draw(pipes);
        window.draw(*game_statistics);

        window.display();

        if (birds.population == 0ULL) {
            genetic_algorithm.rank_fitness(birds);
            genetic_algorithm.apply_mutations(birds);

            game_statistics->new_generation();
            pipes.new_generation();
            birds.reset();
        }

        game_statistics->update();
    }

    return 0;
}