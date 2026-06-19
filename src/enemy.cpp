#include "enemy.hpp"

#include "castengine/world.hpp"
#include "castengine/texture.hpp"
#include "castengine/vec2d.hpp"
#include "castengine/renderer.hpp"
#include "castengine/logger.hpp"

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

void Enemy::Draw(CastEngine::Renderer& render)
{
    render.RenderSprite(mTex, mPos);
}
