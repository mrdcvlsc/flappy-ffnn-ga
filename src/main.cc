#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "bird.hpp"
#include "config.hpp"
#include "pipe.hpp"
#include "gamestats.hpp"
#include "collision.hpp"
#include "genetic_algo.hpp"

std::string lpad(std::string const &raw, char fillchar, size_t width)
{
    static std::ostringstream ss;
    ss << std::right << std::setfill(fillchar) << std::setw(width) << raw;
    std::string padded = ss.str();
    ss.str(std::string());
    ss.clear();
    return padded;
}

int main(int argc, char *args[])
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

    sf::Texture capture_texture;
    size_t      captured_frames = 0;
    bool        capture_frames = false;
    if (argc == 2) {
        if (std::string(args[1]) == "capture") {
            std::cout << "frame capture flag enabled\n";
            capture_frames = true;
            capture_texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
        }
    }

    sf::Color background(19, 235, 220);

    Bird player_bird;
    player_bird.setOutlineColor(sf::Color::Red);
    player_bird.setFillColor(sf::Color::Yellow);

    Birds            birds;
    Pipes            pipes;
    GeneticAlgorithm genetic_algorithm;

    if (!birds.collection[0].neural_net.load_network("fittest.nn")) {
        std::cout << "error loading the fittest network\n";
    } else {
        std::cout << "the previous fittests network is loaded\n";
    }

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
                    window.setView(game_view);
                }

                // cam down
                if (event.key.scancode == sf::Keyboard::Scan::Down) {
                    game_view.move(0.f, VIEW_MOVE_DISTANCE);
                    window.setView(game_view);
                }

                // cam left
                if (event.key.scancode == sf::Keyboard::Scan::Left) {
                    game_view.move(-VIEW_MOVE_DISTANCE, 0.f);
                    window.setView(game_view);
                }

                // cam right
                if (event.key.scancode == sf::Keyboard::Scan::Right) {
                    game_view.move(VIEW_MOVE_DISTANCE, 0.f);
                    window.setView(game_view);
                }

                // zoom in cam
                if (event.key.scancode == sf::Keyboard::Scan::Equal) {
                    game_view.zoom(0.99f);
                    window.setView(game_view);
                }

                // zoom out cam
                if (event.key.scancode == sf::Keyboard::Scan::Hyphen) {
                    game_view.zoom(1.01f);
                    window.setView(game_view);
                }

                // bird jumps
                if (event.key.scancode == sf::Keyboard::Scan::Space) {
                    player_bird.jump();
                }
            }
        }

        genetic_algorithm.get_inputs(birds, pipes);

        while (game_statistics->update_time_elapsed > GameStats::TIME_PER_FRAME) {
            game_statistics->update_time_elapsed -= GameStats::TIME_PER_FRAME;

            pipes.update(GameStats::TIME_PER_FRAME.asSeconds());
            birds.update(GameStats::TIME_PER_FRAME.asSeconds());
            player_bird.update(GameStats::TIME_PER_FRAME.asSeconds());

            game_statistics->record_deaths(birds_collisions(birds, pipes));
            bird_collision(player_bird, pipes);
        }

        window.clear(background);

        window.draw(game_area);
        window.draw(birds);

        if (!player_bird.dead) {
            window.draw(player_bird);
        }

        window.draw(pipes);
        window.draw(*game_statistics);

        window.display();

        if (capture_frames) {
            capture_texture.update(window);
            capture_texture.copyToImage().saveToFile(
              "flappy-ffnn-ga-frame-" + lpad(std::to_string(captured_frames++), '0', 6) + ".jpg"
            );
        }

        if (birds.population == 0ULL && player_bird.dead) {
            genetic_algorithm.rank_fitness(birds);

            if (birds.collection[0].neural_net.save_network("fittest.nn")) {
                std::cout << "fittest network saved\n";
            } else {
                std::cout << "failed to save the fittest network\n";
            }

            genetic_algorithm.apply_mutations(birds);

            game_statistics->new_generation();
            pipes.reset();
            birds.reset();
            player_bird.reset();
        }

        game_statistics->update();
    }

    return 0;
}