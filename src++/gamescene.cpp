#include "gamescene.hpp"

#include "mainmenuscene.hpp"
#include "enemy.hpp"
#include "game.hpp"
#include "logger.hpp"

GameScene::~GameScene()
{
    mMap.Destroy();
    mRenderer.texBank.FreeAll();
}

void GameScene::SpawnEntity(std::unique_ptr<CastEngine::Entity>&& ptr)
{
    mEntManager.PushEntity(std::move(ptr));
}

CastEngine::Map &GameScene::GetMap()
{
    return mMap;
}

bool GameScene::IsWall(vec2d &pos) 
{
    return IsWall(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

bool GameScene::IsWall(int x, int y) 
{
    return mMap[y * mMap.GetWidth() + x] > 0;
}

vec2d GameScene::GetPlayerPos() const
{
    return mPlayer->GetPos();
}

vec2d GameScene::GetPlayerDir() const
{
    return mPlayer->GetDir();
}

void GameScene::OnEnter()
{      
    CastEngine::Map::LoadArgs args;
    
    if(!mMap.LoadMap(mParentGame.GameData.mapFile, args))
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

    mPlayer = dynamic_cast<CastEngine::Player*>(mEntManager.PushEntity(std::make_unique<CastEngine::Player>(*this)));

    mPlayer->SetPos(args.startPos);
    mPlayer->SetRotateSpeed(args.rotateSpeed);
    mPlayer->SetMaxSpeed(args.maxSpeed);

    CastEngine::Enemy* enemy = dynamic_cast<CastEngine::Enemy*>(mEntManager.PushEntity(std::make_unique<CastEngine::Enemy>(*this)));

    enemy->SetTexture(mRenderer.texBank[mRenderer.texBank.BankSize() - 1]);
    enemy->SetPos(vec2d(4, 4));
    enemy->SetMaxSpeed(0.005f);

    mRenderer.SetCamera(mCam);
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

    if(e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
        mPlayer->HandleKeyInput(e);

}

void GameScene::Update(float dtMs)
{
    mEntManager.UpdateEntities(dtMs);

    mCam.Follow(*mPlayer);
}

void GameScene::Draw()
{
    SDL_Color bgColour = {0, 0, 0, 0};
    mRenderer.ClearScreen(bgColour);

    SDL_Color topColour = {40, 40, 40, 255};
    SDL_Color bottColour = {60, 60, 60, 255};
    mRenderer.RenderCeilingAndFloor(topColour, bottColour);

    mRenderer.RenderCameraView(mMap);

    mEntManager.DrawEntities(mRenderer);

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
