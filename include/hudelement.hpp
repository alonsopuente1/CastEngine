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

        std::string mLastValue;
        Texture* mTexture;
        TTF_Font* mFont;

    public:

        void Init(Renderer& rend, TTF_Font* font);
        void Update(Renderer& rend, const std::string& newValue);
        void Draw(Renderer& rend, SDL_Rect dst);


    };

}