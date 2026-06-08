#include "scene.hpp"

#include "button.hpp"
#include "game.hpp"

class MapSelectScene : CastEngine::IScene
{

private:

    std::vector<CastEngine::Button> mMapButtons;

    CastEngine::Button mBackButton;

public:

    MapSelectScene(CastEngine::Game& pGame) : 
        IScene(pGame),
        mBackButton(pGame.GetWindow(), pGame.GetRenderer()) {}

    void Setup() override;
    void HandleEvents(SDL_Event& e) override;
    void Update(float dtMs) override;
    void Draw() override;
    void Destroy() override;
};