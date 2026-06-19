#pragma once

#include "castengine/scene.hpp"
#include "castengine/button.hpp"
#include "castengine/game.hpp"

class MapSelectScene : public CastEngine::IScene
{

private:

    std::vector<CastEngine::Button> mMapButtons;

    CastEngine::Button mBackButton;

public:

    MapSelectScene(CastEngine::Game& pGame) : 
        IScene(pGame),
        mBackButton(pGame.GetWindow(), pGame.GetRenderer()) {}

    ~MapSelectScene();

    void OnEnter() override;
    void HandleEvents(SDL_Event& e) override;
    void Update(float dtMs) override;
    void Draw() override;
    void OnExit() override;

    void OnPause() override;
    void OnResume() override;
};