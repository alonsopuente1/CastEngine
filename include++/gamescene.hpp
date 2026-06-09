#pragma once

#include "scene.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include "map.hpp"
#include "player.hpp"

class GameScene : public CastEngine::IScene
{

private:

    CastEngine::Map mMap;
    CastEngine::Player mPlayer;

public:

    GameScene(CastEngine::Game& pGame) : IScene(pGame), mPlayer(mMap) {}
    ~GameScene();

    void OnEnter() override;
    void HandleEvents(SDL_Event& e) override;
    void Update(float dtMs) override;
    void Draw() override;
    void OnExit() override;

    void OnPause() override;
    void OnResume() override;

};
