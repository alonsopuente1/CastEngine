#pragma once

#include "renderer.hpp"
#include "player.hpp"
#include "scene.hpp"
#include "button.hpp"
#include "game.hpp"
#include "animatedtexture.hpp"

#include <vector>

class MainMenuScene : public CastEngine::IScene
{

public:
    MainMenuScene(CastEngine::Game& pGame) : 
        IScene(pGame), 
        mStartButton(pGame.GetWindow(), pGame.GetRenderer()), 
        mExitButton(pGame.GetWindow(), pGame.GetRenderer()),
        mTex(mRenderer)
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

    CastEngine::AnimatedTexture mTex;

};
