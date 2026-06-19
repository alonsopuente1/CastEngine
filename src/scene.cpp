#include "castengine/scene.hpp"

#include "castengine/game.hpp"

CastEngine::IScene::IScene(Game &pGame)  : mParentGame(pGame), mRenderer(pGame.GetRenderer()), mWindow(pGame.GetWindow()) {}