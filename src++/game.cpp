#include "game.hpp"

#include "logger.hpp"
#include "window.hpp"
#include "scene.hpp"

namespace CastEngine
{
    void Game::HandleEvents()
    {
        SDL_Event e = { 0 };

        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                ShutDown();
                return;
            }

            currentScene->HandleEvents(e);
        }
    }

    void Game::Update(float dtMs)
    {
        currentScene->Update(dtMs);
    }

    void Game::Draw()
    {
        currentScene->Draw();
    }

    Game::Game() : mWindow("CastShooter", 1280, 720), mRenderer(mWindow)
    {
        if(!mWindow.IsInitialised())
        {
            LogMsg(ERROR, "failed to create window in Game constructor\n");
            exit(-1);
        }
    }

    void Game::AddScene(IScene *pScene)
    {
        if(!pScene)
        {
            LogMsg(WARN, "passed null pointer to pScene");
            return;
        }

        mScenes[typeid(*pScene).name()] = (pScene);
    }

    void Game::ChangeScene(const std::string &pSceneName)
    {
        try
        {
            if(currentScene)
                currentScene->Destroy();
            currentScene = mScenes[pSceneName];   
            currentScene->Setup();
            mRenderer.Destroy();
        }
        catch(const std::exception& e)
        {
            LogMsgf(ERROR, "couldnt find scene: %s\n\t\texception msg: %s", e.what());
        }
        
    }
    Window &Game::GetWindow()
    {
        return mWindow;
    }
    Renderer &Game::GetRenderer()
    {
        return mRenderer;
    }
    void Game::Run()
    {

        if(!currentScene)
        {
            LogMsg(ERROR, "NO CURRENT SCENE TO RUN! did you forget to call Game::ChangeScene() to set the current scene?");
            return;
        }

        running = true;

        while(running)
    {

    }

    }
    void Game::ShutDown()
    {
        running = false;
    }
};