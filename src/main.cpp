#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include "config.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

int main() {
    sf::VideoMode video_dimension(WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::RenderWindow window(video_dimension, "Flappy FFNN-GA");

    sf::Color background(19, 235, 220);
    
    sf::RectangleShape bird({BIRD_SIZE, BIRD_SIZE});
    bird.setOrigin(bird.getSize() / 2.f);
    bird.setFillColor(sf::Color::Red);
    bird.setOutlineThickness(3.f);
    bird.setOutlineColor(sf::Color::Black);
    bird.setPosition(BIRD_STARTING_X_POS, BIRD_STARTING_Y_POS);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(background);

        window.draw(bird);

        window.display();
    }

    return 0;
}