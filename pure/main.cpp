#include "Game.hpp"

int main(int argc, char** argv)
{
    Game game;
    if (game.Init() == false)
        return -1;
    game.Run();

    return 0;
}