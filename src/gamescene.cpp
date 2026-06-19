#include "gamescene.hpp"

#include "mainmenuscene.hpp"
#include "enemy.hpp"

#include "castengine/game.hpp"
#include "castengine/logger.hpp"
#include "castengine/fonts.hpp"

GameScene::~GameScene()
{
    mMap.Destroy();
    mRenderer.texBank.FreeAll();
}

// IWorld interface overrides

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

// IScene interface overrides

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

    Enemy* enemy = dynamic_cast<Enemy*>(mEntManager.PushEntity(std::make_unique<Enemy>(*this)));

    enemy->SetTexture(mRenderer.texBank[mRenderer.texBank.BankSize() - 1]);
    enemy->SetPos(vec2d(4, 4));
    enemy->SetMaxSpeed(0.005f);

    mPlayer = dynamic_cast<Player*>(mEntManager.PushEntity(std::make_unique<Player>(*this, mRenderer)));

    mPlayer->SetPos(args.startPos);
    mPlayer->SetRotateSpeed(args.rotateSpeed);
    mPlayer->SetMaxSpeed(args.maxSpeed);

    mRenderer.SetCamera(mCam);

    CastEngine::LoadFont("res/fonts/runescape.ttf");
    mHud.Init(CastEngine::fonts[0]);
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

    if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(mPlayer->TryShoot())
        {
            vec2d origin = mPlayer->GetPos();
            vec2d dir = mPlayer->GetDir();

            CastEngine::Map::RayCastDesc desc;
            mMap.WallRayCast(origin, dir, desc);

            auto [hitEnt, entDist] = mEntManager.RayCast(origin, dir, desc.perpWallDist);

            if(hitEnt)
                hitEnt->Destroy();
        }
    }
}

void GameScene::Update(float dtMs)
{
    mEntManager.UpdateEntities(dtMs);
    mEntManager.RemoveIf([](CastEngine::Entity* ent){
        return !ent->IsAlive();
    });

    mRenderer.UpdateMinimap(mEntManager, mMap);
    mHud.Update(*mPlayer);

    mCam.SetDir(mPlayer->GetDir());
    mCam.SetPos(mPlayer->GetPos());
}

void GameScene::Draw()
{
    SDL_Color bgColour = {0, 0, 0, 0};
    mRenderer.ClearScreen(bgColour);
    mRenderer.ResetDepthBuffer();

    SDL_Color topColour = {40, 40, 40, 255};
    SDL_Color bottColour = {60, 60, 60, 255};
    mRenderer.RenderCeilingAndFloor(topColour, bottColour);

    mRenderer.RenderCameraView(mMap);

    mEntManager.DrawEntities(mRenderer);

    mRenderer.RenderMinimap();

    mHud.Draw();

    mRenderer.Present();
}

void GameScene::OnExit()
{
    mRenderer.texBank.FreeAll();
    CastEngine::CleanupFonts();
}

void GameScene::OnPause()
{
}

void GameScene::OnResume()
{
}
