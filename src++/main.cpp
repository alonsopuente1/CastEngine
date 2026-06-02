#include "game.hpp"

#include "mainmenuscene.hpp"

int main(int argc, char** argv)
{
    CastEngine::Game game;
    MainMenuScene mainMenuScene(game);

    game.AddScene(&mainMenuScene);

    game.ChangeScene(typeid(mainMenuScene).name());

    game.Run();

    return 0;
}