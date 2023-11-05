#include <string>
#include <sstream>
#include <iomanip>

#include <cmath>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include "game.hpp"

std::string lpad(std::string const &raw, char fillchar, size_t width)
{
    static std::ostringstream ss;
    ss << std::right << std::setfill(fillchar) << std::setw(width) << raw;
    std::string padded = ss.str();
    ss.str(std::string());
    ss.clear();
    return padded;
}

Game::Game(std::vector<std::string> const &args, size_t width, size_t height, std::string const &title, size_t fps) :
    window(sf::VideoMode(width, height), title),
    game_area({static_cast<float>(width), static_cast<float>(height)}),
    background(19, 235, 220),
    timePerFrame(sf::seconds(1.f / static_cast<float>(fps))),
    captured_frames(0),
    capture_frames(false),
    game_statistics(std::make_shared<GameStats>()),
    player_bird(),
    birds(),
    pipes(),
    genetic_algorithm()
{
    // check CLI argument if there is a capture flag
    if (args.size() == 2) {
        if (std::string(args[1]) == "capture") {
            std::cerr << "frame capture flag enabled\n";
            capture_frames = true;
            capture_texture.create(width, height);
        }
    }

    // initial window and view configuration.
    sf::View view(sf::FloatRect(0.f, 0.f, width, height));
    window.setView(view);
    window.setFramerateLimit(fps);

    // initialize game area.
    game_area.setFillColor(sf::Color(0, 0, 0, 0));
    game_area.setOutlineColor(sf::Color::Black);
    game_area.setOutlineThickness(2.f);

    // initialize player bird game color.
    player_bird.setOutlineColor(sf::Color::Red);
    player_bird.setFillColor(sf::Color::Yellow);

    // load the previous fittest network.
    if (!birds.collection[0].neural_net.load_network("fittest.nn")) {
        std::cerr << "error loading the fittest network\n";
    } else {
        std::cerr << "the previous fittests network is loaded\n";
    }
}

void Game::process_events()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            genetic_algorithm.rank_fitness(birds);

            if (birds.collection[0].neural_net.save_network("fittest.nn")) {
                std::cerr << "fittest network saved\n";
            } else {
                std::cerr << "failed to save the fittest network\n";
            }

            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            auto game_view = window.getView();

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
}

void Game::run()
{
    sf::Clock clock;
    sf::Time  timeSinceLastUpdate = sf::Time::Zero;
    while (window.isOpen()) {
        process_events();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;

            // in the sfml book the also put another `process_events` call here, IDK if removing it
            // will have an effect in the game.
            // process_events()

            update(timePerFrame);
        }
        render();
        end_of_frame();
    }
}

void Game::update(sf::Time const &dt)
{
    pipes.update(dt.asSeconds());
    birds.update(dt.asSeconds());
    player_bird.update(dt.asSeconds());

    game_statistics->record_deaths(birds_collisions(birds, pipes));
    bird_collision(player_bird, pipes);
}

void Game::render()
{
    window.clear(background);

    window.draw(game_area);
    window.draw(birds);

    if (!player_bird.dead) {
        window.draw(player_bird);
    }

    window.draw(pipes);
    window.draw(*game_statistics);

    window.display();
}

void Game::end_of_frame()
{
    if (capture_frames) {
        capture_texture.update(window);
        capture_texture.copyToImage().saveToFile(
          "flappy-ffnn-ga-frame-" + lpad(std::to_string(captured_frames++), '0', 6) + ".jpg"
        );
    }

    if (birds.population == 0ULL && player_bird.dead) {
        genetic_algorithm.rank_fitness(birds);

        if (birds.collection[0].neural_net.save_network("fittest.nn")) {
            std::cerr << "fittest network saved\n";
        } else {
            std::cerr << "failed to save the fittest network\n";
        }

        genetic_algorithm.apply_mutations(birds);

        game_statistics->new_generation();
        pipes.reset();
        birds.reset();
        player_bird.reset();
    }

    game_statistics->update();
}