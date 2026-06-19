#include "castengine/hudelement.hpp"

#include "castengine/fonts.hpp"
#include "castengine/renderer.hpp"

namespace CastEngine
{

    void HUDElement::Init(TTF_Font *font)
    {
        mFont = font;
    }

    void HUDElement::Update(const std::string &newValue)
    {
        if(newValue == mLastValue)
            return;

        mLastValue = newValue;

        if(mTexture)
        {
            mParentRender.texBank.RemoveByTex(*mTexture);
            mTexture = nullptr;
        }

        SDL_Color white{255, 255, 255, 255};
        mTexture = CreateText(mParentRender, white, mFont, newValue);
    }

    void HUDElement::Draw(SDL_Rect dst)
    {
        if(!mTexture)
            return;
        
        mParentRender.RenderTexture(*mTexture, 
            {0, 0, 
            static_cast<int>(mTexture->GetWidth()), 
            static_cast<int>(mTexture->GetHeight())}, dst);
    }

    void HUDElement::Destroy()
    {
        if(mTexture)
            mParentRender.texBank.RemoveByTex(*mTexture);
        
        mTexture = nullptr;
        mFont = nullptr;
        mLastValue = "";
    }
}
