#include "fonts.hpp"

#include "logger.hpp"
#include "texture.hpp"
#include "renderer.hpp"
#include "window.hpp"

std::string fontPaths[64] = { "res/fonts/runescape.ttf" };


namespace CastEngine
{
    std::vector<TTF_Font*> fonts;

    bool InitFonts()
    {
        if(TTF_Init() < 0)
        {
            LogMsgf(ERROR, "failed to initialise SDL_ttf. TTF_ERROR: %s\n", TTF_GetError());
            return false;
        }

        return true;
    }

    bool LoadFont(const std::string &filePath, int fontSize)
    {
        fonts.push_back(TTF_OpenFont(filePath.c_str(), fontSize));
        if(fonts[fonts.size() - 1] == NULL)
        {
            LogMsgf(ERROR, "failed to load font from path '%s'. TTF_ERROR: %s\n", filePath.c_str(), TTF_GetError());
            fonts.pop_back();
            return false;
        }
        return true;
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
        for(int i = 0; i < static_cast<int>(fonts.size()); i++)
            TTF_CloseFont(fonts[i]);

        TTF_Quit();
    }
};
