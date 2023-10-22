#ifndef MRDCVLSC_STATS_HPP
#define MRDCVLSC_STATS_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class GameStats : public sf::Drawable {
    public:

    static constexpr size_t FRAME_LIMIT = 30;
    static constexpr size_t FONT_SIZE = 12;

    static const sf::Time TIME_PER_FRAME;

    unsigned int generation;
    float        fps;
    sf::Clock    game_clock, fps_clock;
    sf::Time     timeSinceLastUpdate;

    GameStats();

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    /// \brief calculate current FPS & Game Time.
    /// \warning STRICTLY SHOULD ONLY BE CALLED AT THE VERY END OF A FRAME!.
    void update();

    private:

    sf::Text m_fps_txt, m_time_txt, m_generation_txt;
    sf::Font m_font;
};

#endif