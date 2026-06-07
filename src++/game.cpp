#include "game.hpp"

#include "logger.hpp"
#include "window.hpp"
#include "scene.hpp"
#include "fonts.hpp"

#include <SDL2/SDL.h>

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

            mCurrentScene->HandleEvents(e);
        }
    }

    void Game::Update(float dtMs)
    {
        mCurrentScene->Update(dtMs);
    }

    void Game::Draw()
    {
        mCurrentScene->Draw();
    }

    Game::Game() : mWindow("CastShooter", 1280, 720), mRenderer(mWindow), mCurrentScene(nullptr)
    {
        InitFonts();
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            LogMsgf(ERROR, "failed to initialise SDL. SDL_ERROR: %s\n", SDL_GetError());
            exit(-1);
        }

        if(!mWindow.IsInitialised())
        {
            LogMsg(ERROR, "failed to create window in Game constructor\n");
            exit(-1);
        }

        SDL_RendererInfo info = { 0 };

        if(SDL_GetRendererInfo(mRenderer.GetWindow().GetRenderer(), &info) < 0)
        {
            LogMsgf(ERROR, "failed to verify renderer's functionality. SDL_ERROR: %s", SDL_GetError());
            exit(-1);
        }
        
        if(SDL_RENDERER_TARGETTEXTURE && info.flags == 0)
        {
            LogMsg(ERROR, "drivers does not support textures as renderer targets");
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
            if(mCurrentScene)
                mCurrentScene->Destroy();
            mCurrentScene = mScenes[pSceneName];   
            mRenderer.Destroy();
            mCurrentScene->Setup();
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

        if(!mCurrentScene)
        {
            LogMsg(ERROR, "NO CURRENT SCENE TO RUN! did you forget to call Game::ChangeScene() to set the current scene?");
            return;
        }

        running = true;

        int lastTime = static_cast<int>(SDL_GetTicks());

        while(running)
        {
            int startTime = static_cast<int>(SDL_GetTicks());
            int deltaTime = startTime - lastTime;
            lastTime = startTime;

            HandleEvents();
            Update(deltaTime);
            Draw();
        }

    }
    void Game::ShutDown()
    {
        running = false;

        CleanupFonts();
    }
};