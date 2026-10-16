#pragma once

#include "castengine/entity.hpp"

namespace CastEngine
{
    class Texture;
    class SpriteEntity : public Entity
    {

    private:

        Texture* mTex;

    public:

        using Entity::Entity;

        void Draw(Renderer& render) override;

        inline void SetTexture(Texture* tex) { mTex = tex; }
        inline Texture* GetTexture() { return mTex; }
    };
}