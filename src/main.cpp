#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 360), "Flappy FFNN-GA");

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        // window.draw();

        window.display();
    }

    return 0;
}