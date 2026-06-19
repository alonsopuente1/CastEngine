#pragma once

#include "castengine/hudelement.hpp"

#include <SDL2/SDL_ttf.h>

namespace CastEngine
{
    class Renderer;
}

class Player;
class HUD
{

private:

    CastEngine::HUDElement mAmmo;
    CastEngine::HUDElement mHealth;
    CastEngine::HUDElement mScore;

    CastEngine::Renderer& mParentRender;
        
public:

    HUD(CastEngine::Renderer& rend) : mAmmo(rend), mHealth(rend), mScore(rend), mParentRender(rend) {}
    ~HUD() { mAmmo.Destroy(); mHealth.Destroy(); mScore.Destroy(); }
    
    void Init(TTF_Font* font);
    void Update(Player& player);
    void Draw();

};

