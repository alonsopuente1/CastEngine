#pragma once

#include "castengine/map.hpp"
#include "castengine/world.hpp"
#include "castengine/entitymanager.hpp"
#include "castengine/entity.hpp"
#include "castengine/camera.hpp"
#include "castengine/game.hpp"
#include "castengine/scene.hpp"
#include "castengine/renderer.hpp"
#include "castengine/window.hpp"

#include "hud.hpp"
#include "player.hpp"

class GameScene : public CastEngine::IScene, public CastEngine::IWorld
{

private:

    CastEngine::Map mMap;
    Player* mPlayer;

    CastEngine::EntityManager mEntManager;

    CastEngine::Camera mCam{};

    HUD mHud;
    
public:

    GameScene(CastEngine::Game& pGame) : 
        IScene(pGame), 
        mHud(pGame.GetRenderer()) 
        {}
    
    ~GameScene();

    void SpawnEntity(std::unique_ptr<CastEngine::Entity>&& ptr) override;
    CastEngine::Map& GetMap() override;

    bool IsWall(vec2d& pos) override;
    bool IsWall(int x, int y) override;

    vec2d GetPlayerPos() const override;
    vec2d GetPlayerDir() const override;

    void OnEnter() override;
    void HandleEvents(SDL_Event& e) override;
    void Update(float dtMs) override;
    void Draw() override;
    void OnExit() override;

    void OnPause() override;
    void OnResume() override;

};
