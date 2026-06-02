#pragma once

#include "renderer.hpp"
#include "player.hpp"
#include "scene.hpp"

class MainMenuScene : public CastEngine::IScene
{

public:
    MainMenuScene(CastEngine::Game& pGame) : IScene(pGame) {}

    void Setup() override;
    void HandleEvents(SDL_Event& e) override;
    void Update(float dtMs) override;
    void Draw() override;
    void Destroy() override;
private:


};
