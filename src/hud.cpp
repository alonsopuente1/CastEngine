#include "hud.hpp"

#include "gun.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "window.hpp"

namespace CastEngine
{

    void HUD::Init(TTF_Font *font)
    {
        mAmmo.Init(font);
        mHealth.Init(font);
        mScore.Init(font);
    }

    void HUD::Update(Player& player)
    {
        std::string newAmmoText;
        std::string newHealthText;
        std::string newScoreText;

        const Gun& playerGun = player.GetGun();

        newAmmoText = playerGun.GetName() + " " + std::to_string(playerGun.GetAmmo());
        newHealthText = "Health: 100";
        newScoreText = "Score: 0";

        mAmmo.Update(newAmmoText);
        mHealth.Update(newHealthText);
        mScore.Update(newScoreText);
    }

    void HUD::Draw()
    {
        int windowW = mParentRender.GetWindow().GetWidth();
        int windowH = mParentRender.GetWindow().GetHeight();

        int textHeight = windowH / 10;
        int textWidth = windowW / 10;
        
        mAmmo.Draw({
            windowW - textWidth,
            windowH - textHeight,
            textWidth,
            textHeight
        });

        mScore.Draw({
            windowW - textWidth,
            windowH - textHeight * 2,
            textWidth,
            textHeight
        });

        mHealth.Draw({
            windowW - textWidth,
            windowH - textHeight * 3,
            textWidth,
            textHeight
        });
    }

}
