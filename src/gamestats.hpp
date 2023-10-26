#ifndef MRDCVLSC_STATS_HPP
#define MRDCVLSC_STATS_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "bird.hpp"

class GameStats : public sf::Drawable
{
    public:

    static constexpr size_t FRAME_LIMIT = 30;
    static constexpr size_t FONT_SIZE = 12;

    static const sf::Time TIME_PER_FRAME;

    sf::Clock clock_game, clock_fps;
    sf::Time  update_time_elapsed;

    size_t generation, population;
    float  fps;

    GameStats();

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    /// \brief calculate current FPS & Game Time.
    /// \warning STRICTLY SHOULD ONLY BE CALLED AT THE VERY END OF A FRAME!.
    void update();

    void record_deaths(size_t deaths);

    void new_generation();

    private:

    sf::Text text_fps, text_time, text_generation, text_population;
    sf::Font font_style;
};

#endif