#include "mainmenuscene.hpp"

#include "window.hpp"
#include "game.hpp"
#include "texture.hpp"
#include "util.hpp"
#include "fonts.hpp"

void MainMenuScene::Setup()
{
    mParentGame.GetWindow().SetTitle("Main Menu");
    mParentGame.GetRenderer().texBank.PushTexture(CastEngine::Texture(mParentGame.GetWindow(), "res/textures/enemies/cacodemon.png"));

    CastEngine::LoadFont("res/fonts/runescape.ttf", 48);
    
    int rectHeight = mParentGame.GetWindow().GetHeight() / 6;
    int rectWidth = mParentGame.GetWindow().GetWidth() / 2;

    SDL_SetRelativeMouseMode(SDL_FALSE);

    mStartButton.SetPosition({ mParentGame.GetWindow().GetWidth() / 2 - rectWidth / 2, mParentGame.GetWindow().GetHeight() / 2 - rectHeight - 10, rectWidth, rectHeight });
    mStartButton.SetBackgroundColour({0, 0, 0, 100});
    mStartButton.SetText("Play");
    mStartButton.SetOnClick([this]() {
        this->mState = MAPCHOOSE;
    });
    
    mExitButton.SetPosition({ mParentGame.GetWindow().GetWidth() / 2 - rectWidth / 2, mParentGame.GetWindow().GetHeight() / 2 + 10, rectWidth, rectHeight });
    mExitButton.SetBackgroundColour({0, 0, 0, 100});
    mExitButton.SetText("Quit");    
    mExitButton.SetOnClick([this]() {
        this->mParentGame.ShutDown();
    });

    mState = MAINMENU;

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
}

void MainMenuScene::Draw()
{

    SDL_Colour bgColour = { 50, 50, 50, 255 };
    mRenderer.ClearScreen(bgColour);

    mRenderer.RenderTexture(mRenderer.texBank[0], {0, 0, static_cast<int>(mRenderer.texBank[0].GetWidth()), static_cast<int>(mRenderer.texBank[0].GetHeight())}, {100, 100, static_cast<int>(mRenderer.texBank[0].GetWidth()), static_cast<int>(mRenderer.texBank[0].GetHeight())});

    mStartButton.Draw();
    mExitButton.Draw();

    mRenderer.Present();
}

void MainMenuScene::Destroy()
{
}
