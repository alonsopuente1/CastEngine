#include "mainmenuscene.hpp"

#include "window.hpp"
#include "game.hpp"
#include "texture.hpp"
#include "util.hpp"
#include "fonts.hpp"
#include "animatedtexture.hpp"
#include "logger.hpp"

#include "mapselectscene.hpp"

MainMenuScene::~MainMenuScene()
{
    mExitButton.Destroy();
    mStartButton.Destroy();
    mRenderer.texBank.FreeAll();
}

void MainMenuScene::OnEnter()
{
    mWindow.SetTitle("Main Menu");
    mRenderer.texBank.PushTexture(CastEngine::Texture(mParentGame.GetWindow(), "res/textures/enemies/cacodemon.png"));

    if(!CastEngine::LoadFont("res/fonts/runescape.ttf", 48))
    {
        LogMsg(ERROR, "failed to load font");
        mParentGame.ChangeScene<MainMenuScene>();
        return;
    }
    
    int rectHeight = mParentGame.GetWindow().GetHeight() / 6;
    int rectWidth = mParentGame.GetWindow().GetWidth() / 2;

    SDL_SetRelativeMouseMode(SDL_FALSE);

    mStartButton.SetPosition({ 
        mWindow.GetWidth() / 2 - rectWidth / 2, 
        mWindow.GetHeight() / 2 - rectHeight - 10, 
        rectWidth, 
        rectHeight 
    });
    
    mStartButton.SetBackgroundColour({0, 0, 0, 100});
    mStartButton.SetText("Play");
    mStartButton.SetOnClick([this]() {
        this->mParentGame.ChangeScene<MapSelectScene>();
    });
    
    mExitButton.SetPosition({ 
        mWindow.GetWidth() / 2 - rectWidth / 2, 
        mWindow.GetHeight() / 2 + 10, 
        rectWidth, 
        rectHeight 
    });
    
    mExitButton.SetBackgroundColour({0, 0, 0, 100});
    mExitButton.SetText("Quit");
    mExitButton.SetOnClick([this]() {
        this->mParentGame.ShutDown();
    });

    mTest.Init(CastEngine::fonts[0]);
}

void MainMenuScene::HandleEvents(SDL_Event& e)
{

    if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        mStartButton.HandleEvent(e);
        mExitButton.HandleEvent(e);
    }
}

void MainMenuScene::Update(float dtMs)
{
    mTest.Update("Hello, World!");
}

void MainMenuScene::Draw()
{

    SDL_Colour bgColour = { 50, 50, 50, 255 };
    mRenderer.ClearScreen(bgColour);

    mStartButton.Draw();
    mExitButton.Draw();

    mTest.Draw({0, 0, 100, 100});

    mRenderer.Present();
}

void MainMenuScene::OnExit()
{
    CastEngine::CleanupFonts();
    mStartButton.Destroy();
    mExitButton.Destroy();
}

void MainMenuScene::OnPause()
{
}

void MainMenuScene::OnResume()
{
}
