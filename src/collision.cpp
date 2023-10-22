#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

#include "config.hpp"
#include "collision.hpp"
#include "pipe.hpp"

size_t kill_bird_on_collision(Bird &bird, Pipes const &pipes) {
    if (bird.dead) {
        return 0;
    }

    bool hit_floor = bird.getPosition().y > static_cast<float>(WINDOW_HEIGHT);

    // we will only check the first two incomming pipe pairs since
    // those  are the only ones that can possibly collide with the bird.

    bool hit_top_pipe1 = false, hit_btm_pipe1 = false;
    bool hit_top_pipe2 = false, hit_btm_pipe2 = false;

    sf::FloatRect top_pipe1_bounds = pipes.pairs[pipes.front_pipe].top.getGlobalBounds();
    sf::FloatRect btm_pipe1_bounds = pipes.pairs[pipes.front_pipe].btm.getGlobalBounds();

    sf::FloatRect top_pipe2_bounds = pipes.pairs[(pipes.front_pipe + 1) % Pipes::COUNT].top.getGlobalBounds();
    sf::FloatRect btm_pipe2_bounds = pipes.pairs[(pipes.front_pipe + 1) % Pipes::COUNT].btm.getGlobalBounds();

    sf::FloatRect bird_bounds = bird.getGlobalBounds();

    hit_top_pipe1 = bird_bounds.intersects(top_pipe1_bounds);
    hit_top_pipe2 = bird_bounds.intersects(top_pipe2_bounds);

    hit_btm_pipe1 = bird_bounds.intersects(btm_pipe1_bounds);
    hit_btm_pipe2 = bird_bounds.intersects(btm_pipe2_bounds);

    return (bird.dead = hit_floor | hit_top_pipe1 | hit_top_pipe2 | hit_btm_pipe1 | hit_btm_pipe2);
}

size_t kill_birds_on_collision(Birds &birds, Pipes const &pipes) {
    size_t deaths = 0ULL;
    for (auto &bird: birds.birds) {
        deaths += kill_bird_on_collision(bird, pipes);
    }

    birds.population -= deaths;
    return deaths;
}