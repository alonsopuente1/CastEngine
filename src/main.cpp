#include "game.hpp"

#include "mainmenuscene.hpp"

#include "map.hpp"

int main(int argc, char** argv)
{
    CastEngine::Game game;
    
    game.ChangeScene<MainMenuScene>();

    game.Run();

    return 0;
}