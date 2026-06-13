#include "mainmenuscene.hpp"

#include "window.hpp"
#include "game.hpp"
#include "texture.hpp"
#include "util.hpp"
#include "fonts.hpp"
#include "animatedtexture.hpp"

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

    CastEngine::LoadFont("res/fonts/runescape.ttf", 48);
    
    if(!mTex.LoadAnimation("test", "res/textures/guns/dbshotgun/FIREING"))
    {
        mParentGame.ShutDown();
        
    }
    else
    {
        mTex.SetAnimationTime(1000);
        mTex.Loop();
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
    mTex.Update(dtMs);
}

void MainMenuScene::Draw()
{

    SDL_Colour bgColour = { 50, 50, 50, 255 };
    mRenderer.ClearScreen(bgColour);

    CastEngine::Texture* cacText = mRenderer.texBank[0];
    if(cacText)
    {
        mRenderer.RenderTexture(*cacText, {0, 0, static_cast<int>(cacText->GetWidth()), static_cast<int>(cacText->GetHeight())}, {100, 100, static_cast<int>(cacText->GetWidth()), static_cast<int>(cacText->GetHeight())});
    }

    mStartButton.Draw();
    mExitButton.Draw();

    SDL_Rect src = {
        0, 0,
        mTex.GetTexture()->GetWidth(),
        mTex.GetTexture()->GetHeight()
    };


    mRenderer.RenderTexture(*mTex.GetTexture(), src, src);

    mRenderer.Present();
}

void MainMenuScene::OnExit()
{
    CastEngine::CleanupFonts();
    mTex.Destroy();
}

void MainMenuScene::OnPause()
{
}

void MainMenuScene::OnResume()
{
}
