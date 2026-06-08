#include "mapselectscene.hpp"

#include "mainmenuscene.hpp"
#include "util.hpp"
#include "fonts.hpp"

void MapSelectScene::Setup()
{
    // setup the back button to go to main menu    
    int backButtonWidth = 100;
    int backButtonHeight = 100;
    mBackButton.SetPosition({backButtonWidth, mParentGame.GetWindow().GetHeight() - backButtonHeight, backButtonWidth, backButtonHeight});
    mBackButton.SetBackgroundColour({0, 0, 0, 100});
    mBackButton.SetText("Back");
    mBackButton.SetOnClick([this]() {
        this->mParentGame.ChangeScene(typeid(MainMenuScene).name());
    });

    // retrieve all map files and create buttons for them
    std::vector<std::string> mapFiles;
    GetAllFilesInDir("./res/maps/", mapFiles);

    mMapButtons.reserve(mapFiles.size());

    SDL_Rect rect = { 0 };
    for(const std::string& filePath : mapFiles)
    {
        auto onClickFunc = [this, filePath]() {
            this->mParentGame.GameData.mapFile = filePath;
        };

        CastEngine::Button newButton(mParentGame.GetWindow(), mParentGame.GetRenderer());
        newButton.SetBackgroundColour({0, 0, 0, 100});
        newButton.SetPosition(rect);
        newButton.SetOnClick(onClickFunc);
    }

    CastEngine::CreateText(mParentGame.GetRenderer(), {255, 255, 255, 255}, CastEngine::fonts[0], "Map Select")->SetTextureName("mapselecttext");
}

void MapSelectScene::HandleEvents(SDL_Event &e)
{
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

    int dstWidth = mapSelectText.GetWidth() * 10;
    int dstHeight = mapSelectText.GetHeight() * 10;

    render.RenderTexture(*render.texBank["mapselecttext"], 
                        {0, 0, static_cast<int>(mapSelectText.GetWidth()), static_cast<int>(mapSelectText.GetHeight())}, 
                        {mParentGame.GetWindow().GetWidth() / 2 - dstWidth / 2, 10, dstWidth, dstHeight });
    
    render.Present();
}

void MapSelectScene::Destroy()
{
}
