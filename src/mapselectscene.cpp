#include "mapselectscene.hpp"

#include "mainmenuscene.hpp"
#include "gamescene.hpp"

#include "castengine/util.hpp"
#include "castengine/fonts.hpp"
#include "castengine/logger.hpp"

MapSelectScene::~MapSelectScene()
{
    mMapButtons.clear();
    mBackButton.Destroy();
    mRenderer.texBank.FreeAll();
}

void MapSelectScene::OnEnter()
{
    // setup the back button to go to main menu    
    int backButtonWidth = 100;
    int backButtonHeight = 100;
    mBackButton.SetPosition({backButtonWidth, mParentGame.GetWindow().GetHeight() - backButtonHeight, backButtonWidth, backButtonHeight});
    mBackButton.SetBackgroundColour({0, 0, 0, 100});
    mBackButton.SetText("Back");
    mBackButton.SetOnClick([this]() {
        this->mParentGame.ChangeScene<MainMenuScene>();
    });
    
    CastEngine::LoadFont("res/fonts/runescape.ttf", 48);

    // retrieve all map files and create buttons for them
    std::vector<std::string> mapFiles;
    GetAllFilesInDir("./res/maps/", mapFiles);

    SDL_Rect rect = { 0, 0, 0, 0 };
    for(int i = 0; i < static_cast<int>(mapFiles.size()); i++)
    {
        std::string mapFilePath = mapFiles[i];
        auto onClickFunc = [this, mapFilePath]() {
            this->mParentGame.GameData.mapFile = mapFilePath;
            this->mParentGame.ChangeScene<GameScene>();
        };

        int screenWidth = mParentGame.GetWindow().GetWidth();
        int screenHeight = mParentGame.GetWindow().GetHeight();

        int buttonWidth = screenWidth / 5;
        int buttonHeight = screenHeight / 20;

        rect = {
            screenWidth / 2 - buttonWidth / 2,
            buttonHeight * (5 + i),
            buttonWidth,
            buttonHeight
        };

        char cMapName[64];
        fileNameFromPath(mapFiles[i].c_str(), cMapName, sizeof(cMapName));

        std::string mapName(cMapName);

        CastEngine::Button newButton(mParentGame.GetWindow(), mParentGame.GetRenderer());
        newButton.SetBackgroundColour({0, 0, 0, 100});
        newButton.SetPosition(rect);
        newButton.SetOnClick(onClickFunc);
        newButton.SetText(mapName);
        
        mMapButtons.push_back(std::move(newButton));
    }

    CastEngine::CreateText(mParentGame.GetRenderer(), {255, 255, 255, 255}, CastEngine::fonts[0], "Map Select")->SetTextureName("mapselecttext");
}

void MapSelectScene::HandleEvents(SDL_Event &e)
{
    mBackButton.HandleEvent(e);

    for(CastEngine::Button& button : mMapButtons)
        button.HandleEvent(e);
}

void MapSelectScene::Update(float dtMs)
{
}

void MapSelectScene::Draw()
{
    CastEngine::Renderer& render = mParentGame.GetRenderer();
    
    SDL_Color bgColour = {0, 0, 0, 0};
    render.ClearScreen(bgColour);
    

    CastEngine::Texture& mapSelectText = *render.texBank["mapselecttext"];

    int dstWidth = mapSelectText.GetWidth();
    int dstHeight = mapSelectText.GetHeight();

    render.RenderTexture(*render.texBank["mapselecttext"], 
                        {0, 0, static_cast<int>(mapSelectText.GetWidth()), static_cast<int>(mapSelectText.GetHeight())}, 
                        {mParentGame.GetWindow().GetWidth() / 2 - dstWidth / 2, 10, dstWidth, dstHeight });
    
    for(CastEngine::Button& button : mMapButtons)
        button.Draw();

    mBackButton.Draw();

    render.Present();
}

void MapSelectScene::OnExit()
{
    CastEngine::CleanupFonts();
}

void MapSelectScene::OnPause()
{
}

void MapSelectScene::OnResume()
{
}
