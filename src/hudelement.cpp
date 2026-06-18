#include "hudelement.hpp"

#include "fonts.hpp"
#include "renderer.hpp"

namespace CastEngine
{

    void HUDElement::Init(Renderer &rend, TTF_Font *font)
    {
        mFont = font;

    }

    void HUDElement::Update(Renderer &rend, const std::string &newValue)
    {
        if(newValue == mLastValue)
            return;

        mLastValue = newValue;

        if(mTexture)
        {
            rend.texBank.RemoveByTex(*mTexture);
            mTexture = nullptr;
        }

        SDL_Color white{255, 255, 255, 255};
        mTexture = CreateText(rend, white, mFont, newValue);
    }

    void HUDElement::Draw(Renderer &rend, SDL_Rect dst)
    {
        rend.RenderTexture(*mTexture, 
            {0, 0, 
            static_cast<int>(mTexture->GetWidth()), 
            static_cast<int>(mTexture->GetHeight())}, dst);
    }

}
