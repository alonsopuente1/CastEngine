#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

namespace CastEngine
{

    class Texture;
    class Renderer;

    extern std::vector<TTF_Font*> fonts;

    /// @brief initialises the SDL_ttf library. must be called ONCE before using any other font functions. call CleanupFonts to free loaded fonts and quit SDL_ttf when finished
    /// @return true if successful, false if failed
    bool InitFonts();

    bool LoadFont(const std::string& filePath, int fontSize = 16);

    /// @brief creates a texture given text. the texture is stored in the renderers texture bank and a pointer to it is returned. the texture will be freed when the renderer is destroyed, or you can free it manually by using the texture banks remove functions
    /// @param rend renderer to create the texture into
    /// @param colour color of the text
    /// @param font font to use. stored in fonts vector. you can load fonts using LoadFont function
    /// @param text text to create texture from
    /// @return pointer to created texture, or nullptr if failed
    Texture* CreateText(Renderer& rend, SDL_Color colour, TTF_Font* font, const std::string& text);

    /// @brief frees all loaded fonts and quits SDL_ttf. call this when finished using fonts
    void CleanupFonts();

};