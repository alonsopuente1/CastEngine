#include "gamescene.hpp"

#include "mainmenuscene.hpp"

#include "game.hpp"
#include "logger.hpp"

GameScene::~GameScene()
{
    mMap.Destroy();
    mRenderer.texBank.FreeAll();
}

void GameScene::OnEnter()
{      
    CastEngine::Map::LoadArgs args;
    
    if(!mMap.NewLoadMap(mParentGame.GameData.mapFile, args))
    {
        LogMsgf(ERROR, "failed to load map '%s'", mParentGame.GameData.mapFile.c_str());
        mParentGame.ChangeScene<MainMenuScene>();
        return;
    }

    static std::string texturePaths[] = {
        "res/textures/env/bluestone.png",
        "res/textures/env/colorstone.png",
        "res/textures/env/greystone.png",
        "res/textures/env/mossy.png",
        "res/textures/env/purplestone.png",
        "res/textures/env/redbrick.png",
        "res/textures/env/wood.png",
        "res/textures/env/eagle.png",
        "res/textures/player.png",
        "res/textures/guns/FIST.png",
        "res/textures/enemies/cacodemon.png"
    };

    int numTextures = (static_cast<int>(sizeof(texturePaths)) / static_cast<int>(sizeof(texturePaths[0])));
    for(int i = 0; i < numTextures; i++)
    {
        mRenderer.texBank.PushTexture(CastEngine::Texture(mWindow, texturePaths[i]));
    }

    mPlayer.SetPos(args.startPos);
}

void GameScene::HandleEvents(SDL_Event& e)
{

    if(e.type == SDL_KEYDOWN)
    {
        switch(e.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            mParentGame.ChangeScene<MainMenuScene>();
            break;
        }
    }

}

void GameScene::Update(float dtMs)
{
}

void GameScene::Draw()
{
    SDL_Color bgColour = {0, 0, 0, 0};
    mRenderer.ClearScreen(bgColour);

    SDL_Color topColour = {40, 40, 40, 255};
    SDL_Color bottColour = {60, 60, 60, 255};
    mRenderer.RenderCeilingAndFloor(topColour, bottColour);

    mRenderer.RenderPlayerView(mPlayer, mMap);

    mRenderer.Present();
}

void GameScene::OnExit()
{
}

void GameScene::OnPause()
{
}

void GameScene::OnResume()
{
}
