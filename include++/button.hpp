#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <functional>

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
        
        std::function<void()> mOnClick;

    public:

        Button(Window& pWnd, Renderer& pRend);
        ~Button();

        void SetText(const std::string& newText);
        void SetPosition(SDL_Rect newPos);
        void SetBackgroundColour(SDL_Color newColour);
        inline void SetOnClick(std::function<void()> onClick) { mOnClick = onClick; }

        void HandleEvent(const SDL_Event& e);

        void Draw();

        // destroys the texture of the button, nothing else since
        // only texture is dynamically allocated
        void Destroy();

    };

};