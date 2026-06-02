#pragma once

#include "game.hpp"

namespace CastEngine
{

    class Renderer;
    class Window;

    /// @brief scene class interface to implement own scenes
    class IScene {
    
        
        public:
        
        IScene(Game& pGame) : mParentGame(pGame), mRenderer(pGame.GetRenderer()), mWindow(pGame.GetWindow()) {} 
        
        virtual void Setup() = 0;
        virtual void HandleEvents(SDL_Event e) = 0;
        virtual void Update(float dtMs) = 0;
        virtual void Draw() = 0;
        virtual void Destroy() = 0;
        
    protected:

        Game& mParentGame;

        Renderer& mRenderer;
        Window& mWindow;
    };
};