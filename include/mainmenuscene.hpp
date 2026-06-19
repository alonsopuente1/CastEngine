#pragma once

#include "scene.hpp"
#include "button.hpp"
#include "game.hpp"
#include "hudelement.hpp"

class MainMenuScene : public CastEngine::IScene
{

public:
    MainMenuScene(CastEngine::Game& pGame) : 
        IScene(pGame), 
        mStartButton(pGame.GetWindow(), pGame.GetRenderer()), 
        mExitButton(pGame.GetWindow(), pGame.GetRenderer()),
        mTest(pGame.GetRenderer())
        {}

    ~MainMenuScene();

    void OnEnter() override;
    void HandleEvents(SDL_Event& e) override;
    void Update(float dtMs) override;
    void Draw() override;
    void OnExit() override;

    void OnPause() override;
    void OnResume() override;

private:

    CastEngine::Button mStartButton;
    CastEngine::Button mExitButton;

    CastEngine::HUDElement mTest;

};
