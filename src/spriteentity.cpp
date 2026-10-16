#include "castengine/spriteentity.hpp"

#include "castengine/texture.hpp"
#include "castengine/renderer.hpp"

namespace CastEngine
{
    void SpriteEntity::Draw(Renderer& render) 
    {
        if(mTex)
            render.RenderSprite(mTex, mPos);
    }
}