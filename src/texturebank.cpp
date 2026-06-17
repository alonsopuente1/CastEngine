#include "texturebank.hpp"

#include "logger.hpp"

namespace CastEngine
{

    Texture* TextureBank::operator[] (int i )
    {
        try
        {
            return mTextures.at(i).get();
        }
        catch(const std::exception& e)
        {
            LogMsgf(ERROR, "attempted to access texture outside of bounds. EXCEPTION: %s", e.what());
            return nullptr;
        }
        
    }

    Texture* TextureBank::operator[](const std::string &str)
    {
        if(str.empty())
            return nullptr;

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
        if(!tex.IsInitialised())
        {
            LogMsg(WARN, "attempting to push empty texture to texbank");
            return nullptr;
        }

        mTextures.emplace_back(std::make_unique<Texture>(std::move(tex)));
        return mTextures.back().get();
    }
};