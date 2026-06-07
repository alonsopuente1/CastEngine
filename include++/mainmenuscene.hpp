#pragma once

#include "renderer.hpp"
#include "player.hpp"
#include "scene.hpp"
#include "button.hpp"
#include "game.hpp"

#include <vector>

class MainMenuScene : public CastEngine::IScene
{

public:
    MainMenuScene(CastEngine::Game& pGame) : 
        IScene(pGame), 
        mStartButton(pGame.GetWindow(), pGame.GetRenderer()), 
        mExitButton(pGame.GetWindow(), pGame.GetRenderer()), 
        mBackButton(pGame.GetWindow(), pGame.GetRenderer()) {}

    void Setup() override;
    void HandleEvents(SDL_Event& e) override;
    void Update(float dtMs) override;
    void Draw() override;
    void Destroy() override;
private:

    enum MainMenuState
    {
        MAINMENU,
        MAPCHOOSE
    } mState;

    CastEngine::Button mStartButton;
    CastEngine::Button mExitButton;
    CastEngine::Button mBackButton;

    std::vector<CastEngine::Button> mMapFileButtons;
};
