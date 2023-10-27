#include <algorithm>
#include <string>

#include "gamestats.hpp"

const sf::Time GameStats::TIME_PER_FRAME = sf::seconds(1.f / static_cast<float>(GameStats::FRAME_LIMIT));

GameStats::GameStats() :
    clock_game(),
    clock_fps(),
    update_time_elapsed(sf::Time::Zero),
    generation(1u),
    population(Birds::MAX_POPULATION),
    fps(0.f),
    highest_gametime(0.f)
{
    if (!font_style.loadFromFile("calibril.ttf")) {
        throw std::runtime_error("Error loading calibril.ttf");
    }

    text_fps = sf::Text("FPS : " + std::to_string(fps), font_style, FONT_SIZE);
    text_fps.setFillColor(sf::Color::Black);

    text_time = sf::Text("Game Time : (0 | 0)", font_style, FONT_SIZE);
    text_time.setFillColor(sf::Color::Black);
    text_time.setPosition({text_time.getPosition().x, text_fps.getPosition().y + text_fps.getCharacterSize()});

    text_generation = sf::Text("Generation : " + std::to_string(generation), font_style, FONT_SIZE);
    text_generation.setFillColor(sf::Color::Black);
    text_generation.setPosition(
      {text_generation.getPosition().x, text_time.getPosition().y + text_time.getCharacterSize()}
    );

    text_population = sf::Text(
      "Population : " + std::to_string(population) + '/' + std::to_string(Birds::MAX_POPULATION), font_style, FONT_SIZE
    );
    text_population.setFillColor(sf::Color::Black);
    text_population.setPosition(
      {text_population.getPosition().x, text_generation.getPosition().y + text_generation.getCharacterSize()}
    );
}

void GameStats::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(text_fps, states);
    target.draw(text_time, states);
    target.draw(text_generation, states);
    target.draw(text_population, states);
}

void GameStats::update()
{
    text_fps.setString("FPS : " + std::to_string(static_cast<unsigned int>(fps)));
    text_time.setString(
      "Game Time : (" + std::to_string(static_cast<size_t>(highest_gametime)) + "s | " +
      std::to_string(static_cast<size_t>(clock_game.getElapsedTime().asSeconds())) + "s)"
    );
    fps = 1.f / clock_fps.getElapsedTime().asMilliseconds() * 1000.f;
}

void GameStats::record_deaths(size_t deaths)
{
    population -= deaths;
    text_population.setString(
      "Population : " + std::to_string(population) + '/' + std::to_string(Birds::MAX_POPULATION)
    );
}

void GameStats::new_generation()
{
    generation++;
    text_generation.setString("Generation : " + std::to_string(generation));

    population = Birds::MAX_POPULATION;
    fps = 0.f;

    highest_gametime = std::max(highest_gametime, clock_game.getElapsedTime().asSeconds());
    clock_game.restart();
    clock_fps.restart();

    update_time_elapsed = sf::Time::Zero;
}