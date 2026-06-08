#include "game.hpp"

#include "mainmenuscene.hpp"

int main(int argc, char** argv)
{
    CastEngine::Game game;
    
    game.ChangeScene<MainMenuScene>();

    game.Run();

    return 0;
}