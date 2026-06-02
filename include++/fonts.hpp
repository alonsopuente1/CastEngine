#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

namespace CastEngine
{

    class Texture;
    class Renderer;

    TTF_Font* fonts[64];

    void InitFonts();

    Texture* CreateText(Renderer& rend, SDL_Color colour, TTF_Font* font, const std::string& text);

    void CleanupFonts();

};