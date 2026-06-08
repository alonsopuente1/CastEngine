#pragma once

#include "window.hpp"
#include "renderer.hpp"

#include <unordered_map>
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

        std::unordered_map<std::string, IScene*> mScenes;
        
        IScene* mCurrentScene;

        void HandleEvents();
        void Update(float dtMs);
        void Draw();
        
        public:
        
        Game();

        void AddScene(IScene* pScene);

        void ChangeScene(const std::string& pSceneName);

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
    
};