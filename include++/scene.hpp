#pragma once

typedef union SDL_Event SDL_Event;

namespace CastEngine
{
    class Game;
    class Renderer;
    class Window;

    /// @brief scene class interface to implement own scenes
    class IScene {
    
    protected:

        Game& mParentGame;

        Renderer& mRenderer;
        Window& mWindow;
        
    public:
        
        IScene(Game& pGame); 
        
        virtual void Setup() = 0;
        virtual void HandleEvents(SDL_Event& e) = 0;
        virtual void Update(float dtMs) = 0;
        virtual void Draw() = 0;
        virtual void Destroy() = 0;
        
    };
};