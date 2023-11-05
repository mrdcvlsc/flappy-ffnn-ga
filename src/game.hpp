#ifndef MRDCVLSC_GAME_HPP
#define MRDCVLSC_GAME_HPP

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "config.hpp"
#include "bird.hpp"
#include "pipe.hpp"
#include "gamestats.hpp"
#include "collision.hpp"
#include "genetic_algo.hpp"

std::string lpad(std::string const &raw, char fillchar, size_t width);

class Game
{
    public:

    Game(std::vector<std::string> const &args, size_t width, size_t height, std::string const &title, size_t fps = 60);

    // uses fixed time steps.
    void run();

    private:

    void process_events();
    void update(sf::Time const &dt);
    void render();
    void end_of_frame();

    private:

    sf::RenderWindow   window;
    sf::RectangleShape game_area;
    sf::Color          background;
    sf::Texture        capture_texture;
    sf::Time           timePerFrame;
    size_t             captured_frames;
    bool               capture_frames;

    // game entities
    std::shared_ptr<GameStats> game_statistics;

    Bird             player_bird;
    Birds            birds;
    Pipes            pipes;
    GeneticAlgorithm genetic_algorithm;
};

#endif