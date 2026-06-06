#include "texturebank.hpp"

namespace CastEngine
{

    Texture& TextureBank::operator[] (int i )
    {
        return mTextures[i];
    }

    Texture* TextureBank::operator[](const std::string &str)
    {
        for(Texture& tex : mTextures)
            if (tex.GetTextureName() == str)
                return &tex;

        return NULL;
    }

    void TextureBank::FreeAll()
    {
        mTextures.clear();
    }

    void TextureBank::RemoveByName(const std::string &pName)
    {
        for(int i = 0; i < mTextures.size(); i++)
        {
            if(mTextures[i].GetTextureName() == pName)
            {
                mTextures.erase(mTextures.begin() + i);
                return;
            }
        }
    }

    void TextureBank::RemoveByTex(const Texture &pTex)
    {
        for(int i = 0; i < mTextures.size(); i++)
        {
            if(mTextures[i].GetTexture() == pTex.GetTexture())
            {
                mTextures.erase(mTextures.begin() + i);
                return;
            }
        }
    }

    void TextureBank::PushTexture(Texture &&tex)
    {
        mTextures.push_back(std::move(tex));
    }
};