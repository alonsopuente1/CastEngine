#include "scene.hpp"

#include "game.hpp"

CastEngine::IScene::IScene(Game &pGame)  : mParentGame(pGame), mRenderer(pGame.GetRenderer()), mWindow(pGame.GetWindow()) {}