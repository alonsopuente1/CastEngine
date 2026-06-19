#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

namespace CastEngine
{

    class Texture;
    class Renderer;
    class HUDElement
    {

    private:

        Renderer& mParentRender;
        std::string mLastValue = "";
        Texture* mTexture = nullptr;
        TTF_Font* mFont = nullptr;

    public:
        HUDElement(Renderer& rend) : mParentRender(rend) {}
        ~HUDElement() { Destroy(); }

        void Init(TTF_Font* font);
        void Update(const std::string& newValue);
        void Draw(SDL_Rect dst);

        void Destroy();
    };

}