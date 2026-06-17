#pragma once

#include "scene.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include "map.hpp"
#include "player.hpp"
#include "world.hpp"
#include "entitymanager.hpp"
#include "entity.hpp"
#include "camera.hpp"

class GameScene : public CastEngine::IScene, public CastEngine::IWorld
{

private:

    CastEngine::Map mMap;
    CastEngine::Player* mPlayer;

    CastEngine::EntityManager mEntManager;

    CastEngine::Camera mCam{};
public:

    GameScene(CastEngine::Game& pGame) : IScene(pGame) {}
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
