#include "mainmenuscene.hpp"

#include "window.hpp"
#include "game.hpp"
#include "texture.hpp"

void MainMenuScene::Setup()
{
    mParentGame.GetWindow().SetTitle("Main Menu");
    mParentGame.GetRenderer().texBank.PushTexture(CastEngine::Texture(mParentGame.GetWindow(), "res/textures/enemies/cacodemon.png"));
}

void MainMenuScene::HandleEvents(SDL_Event e)
{
    

}

void MainMenuScene::Update(float dtMs)
{
}

void MainMenuScene::Draw()
{

    SDL_Colour bgColour = { 50, 50, 50, 255 };
    mRenderer.ClearScreen(bgColour);

    mRenderer.RenderTexture(mRenderer.texBank[0], {0, 0, static_cast<int>(mRenderer.texBank[0].GetWidth()), static_cast<int>(mRenderer.texBank[0].GetHeight())}, {100, 100, static_cast<int>(mRenderer.texBank[0].GetWidth()), static_cast<int>(mRenderer.texBank[0].GetHeight())});

    mRenderer.Present();
}

void MainMenuScene::Destroy()
{
}