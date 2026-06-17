#include "entity.hpp"

#include "world.hpp"

void CastEngine::Entity::ResolveWallCollision(vec2d newPos)
{
        int left, right, top, bottom;
        bool changeX = false;
        bool changeY = false;
        // resolve x

        left = static_cast<int>(newPos.x - mRadius);
        right = static_cast<int>(newPos.x + mRadius);
        top = static_cast<int>(mPos.y - mRadius);
        bottom = static_cast<int>(mPos.y + mRadius);


        if(
            !mWorld.IsWall(left, top) &&
            !mWorld.IsWall(left, bottom) &&
            !mWorld.IsWall(right, top) &&
            !mWorld.IsWall(right, bottom))
        {
            changeX = true;
        }

        // resolve y

        left = static_cast<int>(mPos.x - mRadius);
        right = static_cast<int>(mPos.x + mRadius);
        top = static_cast<int>(newPos.y - mRadius);
        bottom = static_cast<int>(newPos.y + mRadius);

        if(
            !mWorld.IsWall(left, top) &&
            !mWorld.IsWall(left, bottom) &&
            !mWorld.IsWall(right, top) &&
            !mWorld.IsWall(right, bottom))
        {
            changeY = true;
        }

        if(changeX)
            mPos.x = newPos.x;
        if(changeY)
            mPos.y = newPos.y;
    }