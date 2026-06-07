#include "texturebank.hpp"

namespace CastEngine
{

    Texture& TextureBank::operator[] (int i )
    {
        return *mTextures[i];
    }

    Texture* TextureBank::operator[](const std::string &str)
    {
        for(const auto& tex : mTextures)
            if (tex->GetTextureName() == str)
                return tex.get();

        return nullptr;
    }

    void TextureBank::FreeAll()
    {
        mTextures.clear();
    }

    void TextureBank::RemoveByName(const std::string &pName)
    {
        for(size_t i = 0; i < mTextures.size(); i++)
        {
            if(mTextures[i]->GetTextureName() == pName)
            {
                mTextures.erase(mTextures.begin() + i);
                return;
            }
        }
    }

    void TextureBank::RemoveByTex(const Texture &pTex)
    {
        for(size_t i = 0; i < mTextures.size(); i++)
        {
            if(mTextures[i]->GetTexture() == pTex.GetTexture())
            {
                mTextures.erase(mTextures.begin() + i);
                return;
            }
        }
    }

    Texture* TextureBank::PushTexture(Texture &&tex)
    {
        mTextures.emplace_back(std::make_unique<Texture>(std::move(tex)));
        return mTextures.back().get();
    }
};