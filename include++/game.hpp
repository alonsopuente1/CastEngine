#pragma once

#include "window.hpp"
#include "renderer.hpp"

#include <vector>
#include <memory>

namespace CastEngine
{
    
    class IScene;
    class Game
    {
        
    private:
        
        Window mWindow;
        Renderer mRenderer;
        
        bool running;

        std::vector<std::unique_ptr<IScene>> mSceneStack;

        IScene* CurrentScene() const;   

        void HandleEvents();
        void Update(float dtMs);
        void Draw();
        
    public:
        
        Game();

        template<typename T>
        void ChangeScene();

        template<typename T>
        void PushScene();

        void PopScene();

        Window& GetWindow();
        Renderer& GetRenderer();

        void Run();

        void ShutDown();
        
        struct
        {
            // stores the file path to the map to load when changing to GameScene
            std::string mapFile;
        } GameData;
    };

    template <typename T>
    void Game::ChangeScene()
    {
        mSceneStack.clear();

        mSceneStack.push_back(std::make_unique<T>(*this));
        mSceneStack.back().get()->Setup();
    }
    
    template <typename T>
    void Game::PushScene()
    {
        mSceneStack.push_back(std::make_unique<T>(*this));
    }

};