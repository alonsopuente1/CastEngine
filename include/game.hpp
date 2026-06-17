#pragma once

#include "window.hpp"
#include "renderer.hpp"

#include <vector>
#include <queue>
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

        void HandleEvents();
        void Update(float dtMs);
        void Draw();


        // scene related 

        std::vector<std::unique_ptr<IScene>> mSceneStack;
        IScene* CurrentScene() const;   
        void ProcessSceneCommands();

        struct SceneCommand
        {
            enum class Type
            {
                Change,
                Push,
                Pop
            };

            Type type;
            std::unique_ptr<IScene> scene;
        };

        std::queue<SceneCommand> mPendingSceneCommands;        

    public:
        
        Game();

        
        Window& GetWindow();
        Renderer& GetRenderer();

        void Run();

        void ShutDown();
        
        struct
        {
            // stores the file path to the map to load when changing to GameScene
            std::string mapFile;
        } GameData;

        // scene related 

        template<typename T>
        void ChangeScene();

        template<typename T>
        void PushScene();

        void PopScene();
    };


    // templated function implementations


    template <typename T>
    void Game::ChangeScene()
    {
        mPendingSceneCommands.push({
            SceneCommand::Type::Change, 
            std::make_unique<T>(*this)
        });
    }
    
    template <typename T>
    void Game::PushScene()
    {
        mPendingSceneCommands.push({
            SceneCommand::Type::Push, 
            std::make_unique<T>(*this)
        });
    }

};