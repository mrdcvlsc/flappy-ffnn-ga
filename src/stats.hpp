#ifndef MRDCVLSC_STATS_HPP
#define MRDCVLSC_STATS_HPP

#include <exception>
#include <iostream>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class GameStats : public sf::Drawable {
    private:

    sf::Text m_fps_txt, m_time_txt;
    sf::Font m_font;

    public:

    static constexpr size_t FRAME_LIMIT = 30;
    static constexpr size_t FONT_SIZE = 12;

    static const sf::Time TIME_PER_FRAME;

    float     fps;
    sf::Clock game_clock, fps_clock;
    sf::Time  timeSinceLastUpdate;

    GameStats() : fps(0.f), game_clock(), fps_clock(), timeSinceLastUpdate(sf::Time::Zero) {
        if (!m_font.loadFromFile("calibril.ttf")) {
            throw std::runtime_error("Error loading calibril.ttf");
        }

        m_fps_txt = sf::Text("FPS : " + std::to_string(fps), m_font, FONT_SIZE);
        m_fps_txt.setFillColor(sf::Color::Black);

        m_time_txt = sf::Text("Game Time : 0", m_font, FONT_SIZE);
        m_time_txt.setFillColor(sf::Color::Black);
        m_time_txt.setPosition({m_time_txt.getPosition().x, m_fps_txt.getPosition().y + m_fps_txt.getCharacterSize()});
    }

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(m_fps_txt, states);
        target.draw(m_time_txt, states);
    }

    /// \brief calculate current FPS & Game Time.
    /// \warning STRICTLY SHOULD ONLY BE CALLED AT THE VERY END OF A FRAME!.
    void update() {
        m_fps_txt.setString("FPS : " + std::to_string(static_cast<unsigned int>(fps)));
        m_time_txt.setString("Game Time : " + std::to_string(game_clock.getElapsedTime().asSeconds()));
        fps = 1.f / fps_clock.getElapsedTime().asMilliseconds() * 1000.f;
    }
};

const sf::Time GameStats::TIME_PER_FRAME = sf::seconds(1.f / static_cast<float>(GameStats::FRAME_LIMIT));

#endif