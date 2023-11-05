#include <vector>

#include "game.hpp"

int main(int argc, char *args[])
{
    std::vector<std::string> arguments;
    for (int i = 0; i < argc; ++i) {
        arguments.push_back(args[i]);
    }

    Game game(arguments, WINDOW_WIDTH, WINDOW_HEIGHT, "Flappy FFNN GA");
    game.run();

    return 0;
}