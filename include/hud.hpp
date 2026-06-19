#pragma once

#include "hudelement.hpp"

#include <SDL2/SDL_ttf.h>

namespace CastEngine
{

    class Renderer;
    class Player;
    class HUD
    {

    private:

        HUDElement mAmmo;
        HUDElement mHealth;
        HUDElement mScore;

        Renderer& mParentRender;
        
    public:

        HUD(Renderer& rend) : mAmmo(rend), mHealth(rend), mScore(rend), mParentRender(rend) {}
        ~HUD() { mAmmo.Destroy(); mHealth.Destroy(); mScore.Destroy(); }

        void Init(TTF_Font* font);
        void Update(Player& player);
        void Draw();

    };

}