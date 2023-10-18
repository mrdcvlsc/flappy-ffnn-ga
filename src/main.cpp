#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>

#include "bird.hpp"
#include "config.hpp"
#include "pipe.hpp"
#include "stats.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <string>

int main() {
    static GameStats game_stats;

    sf::View           game_view(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));
    sf::VideoMode      video_dimension(WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::RectangleShape game_area({WINDOW_WIDTH, WINDOW_HEIGHT});

    game_area.setFillColor(sf::Color(0, 0, 0, 0));
    game_area.setOutlineColor(sf::Color::Black);
    game_area.setOutlineThickness(2.f);

    sf::RenderWindow window(video_dimension, "Flappy FFNN-GA");
    window.setView(game_view);
    window.setFramerateLimit(game_stats.FRAME_LIMIT);

    sf::Color background(19, 235, 220);

    Bird bird;

    Pipe p1(200, WINDOW_WIDTH, pipe_type::top);
    Pipe p2(100, WINDOW_WIDTH, pipe_type::bottom);

    while (window.isOpen()) {
        sf::Time dt = game_stats.fps_clock.restart();
        game_stats.timeSinceLastUpdate += dt;

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

                window.setView(game_view);
            }
        }

        while (game_stats.timeSinceLastUpdate > game_stats.TIME_PER_FRAME) {
            game_stats.timeSinceLastUpdate -= game_stats.TIME_PER_FRAME;

            p1.update(game_stats.TIME_PER_FRAME.asSeconds());
            p2.update(game_stats.TIME_PER_FRAME.asSeconds());
        }

        window.clear(background);

        window.draw(game_area);

        // for (int i = 0; i < 50'000; ++i)
        window.draw(bird);

        window.draw(p1);
        window.draw(p2);

        window.draw(game_stats);

        window.display();

        game_stats.update();
    }

    return 0;
}