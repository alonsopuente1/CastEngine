#pragma once

#include <string>
#include <SDL2/SDL.h>

namespace CastEngine
{
    class Window;
    class Renderer;
    class Texture;

    class Button
    {

    private:

        Window& mParentWindow;
        Renderer& mParentRenderer;

        SDL_Rect mPos;

        SDL_Color mBackgroundColour;

        Texture* mText;

    public:

        Button(Window& pWnd, Renderer& pRend);
        ~Button();

        void SetText(const std::string& newText);
        void SetPosition(SDL_Rect newPos);
        void SetBackgroundColour(SDL_Color newColour);

        void Draw();

        // destroys the texture of the button, nothing else since
        // only texture is dynamically allocated
        void Destroy();

    };

};