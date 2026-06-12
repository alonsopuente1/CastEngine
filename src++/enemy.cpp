#include "enemy.hpp"

#include "world.hpp"
#include "texture.hpp"
#include "vec2d.hpp"
#include "renderer.hpp"

#include "logger.hpp"

namespace CastEngine
{
    void Enemy::Think()
    {
        mAcc = vec2d(-0.0005f, 0.0f);
    }

    void Enemy::Update(float dtMs)
    {
        Think();

        mVel += mAcc * dtMs;
        if(mVel.GetMagnitude() > mMaxSpeed)
            mVel.SetMagnitude(mMaxSpeed);

        ResolveWallCollision(mPos + (mVel * dtMs));
    }

    void Enemy::Draw(Renderer& render)
    {
        render.RenderSprite(mTex, mPos);
    }
}