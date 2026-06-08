#include "game.hpp"

#include "logger.hpp"
#include "window.hpp"
#include "scene.hpp"
#include "fonts.hpp"

#include <SDL2/SDL.h>

#include "mainmenuscene.hpp"
#include "mapselectscene.hpp"

namespace CastEngine
{
    IScene *Game::CurrentScene() const
    {
        return mSceneStack.empty() ? nullptr : mSceneStack.back().get();
    }

    void Game::ProcessSceneCommands()
    {

        while(!mPendingSceneCommands.empty())
        {
            SceneCommand& command = mPendingSceneCommands.front();

            switch(command.type)
            {
            case SceneCommand::Type::Change:
                mSceneStack.clear();
                mSceneStack.push_back(std::move(command.scene));
                break;
            case SceneCommand::Type::Push:
                mSceneStack.push_back(std::move(command.scene));
                break;
            case SceneCommand::Type::Pop:
                mSceneStack.pop_back();
                break;
            }

            mPendingSceneCommands.pop();
        }

    }

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

            CurrentScene()->HandleEvents(e);
        }
    }

    void Game::Update(float dtMs)
    {
        CurrentScene()->Update(dtMs);
    }

    void Game::Draw()
    {
        CurrentScene()->Draw();
    }

    Game::Game() : mWindow("CastShooter", 1280, 720), mRenderer(mWindow)
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

        SDL_RendererInfo info;

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

    void Game::PopScene()
    {
        mSceneStack.pop_back();
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

        if(!CurrentScene())
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

            ProcessSceneCommands();

            Draw();
        }

    }
    void Game::ShutDown()
    {
        running = false;

        CleanupFonts();
    }

    
    
};