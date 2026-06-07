#pragma once

#include "texture.hpp"

#include <memory>
#include <vector>

namespace CastEngine
{
    class TextureBank
    {

    public:

        Texture& operator[] (int i);
        Texture* operator[] (const std::string& str);
        
        void FreeAll();
        void RemoveByName(const std::string& pName);
        void RemoveByTex(const Texture& pTex);

        /// @brief size of the internal vector of Texture's
        /// @return size of vector
        inline size_t BankSize() const { return mTextures.size(); };

        Texture* PushTexture(Texture&& tex);

    private:

        std::vector<std::unique_ptr<Texture>> mTextures;

    };
};