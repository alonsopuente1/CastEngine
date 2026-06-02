#include "fonts.hpp"

#include "logger.h"
#include "texture.hpp"
#include "renderer.hpp"
#include "window.hpp"

std::string fontPaths[64] = { "res/fonts/runescape.ttf" };


namespace CastEngine
{
    TTF_Font* fonts[64] = { nullptr };
    void InitFonts()
    {
        int numOfFilePaths = sizeof(fontPaths) / sizeof(std::string);
    
        int iterations = numOfFilePaths > 64 ? 64 : numOfFilePaths;
    
        for(int i = 0; i < iterations; i++)
        {
            fonts[i] = TTF_OpenFont(fontPaths[i].c_str(), 16);
            if(!fonts[i])
                LogMsgf(WARN, "failed to load font at filepath '%s'. TTF_ERROR: %s\n", fontPaths[i].c_str(), TTF_GetError());
        }
    }
    
    Texture* CreateText(Renderer &rend, SDL_Color colour, TTF_Font *font, const std::string &text)
    {
        SDL_Texture* texture = nullptr;
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), colour);
        if(!surface)
        {
            LogMsgf(ERROR, "failed to create surface for text '%s'. TTF_ERROR: %s\n", text.c_str(), TTF_GetError());
            return nullptr;
        }

        texture = SDL_CreateTextureFromSurface(rend.GetWindow().GetRenderer(), surface);
        SDL_FreeSurface(surface);
        if(!texture)
        {
            LogMsgf(ERROR, "failed to create texture for text '%s'. SDL_ERROR: %s\n", text.c_str(), SDL_GetError());
            return nullptr;
        }

        rend.texBank.PushTexture(Texture(rend.GetWindow(), texture));
        return &rend.texBank[rend.texBank.BankSize() - 1];

    }

    void CleanupFonts()
    {
        for(int i = 0; i < static_cast<int>(sizeof(fonts) / sizeof(*fonts)); i++)
            TTF_CloseFont(fonts[i]);
    }
};
