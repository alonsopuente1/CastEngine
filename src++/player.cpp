#include "player.hpp"

#include "logger.hpp"
#include "map.hpp"

#include "world.hpp"

namespace CastEngine
{
    void Player::HandleKeyInput(const SDL_Event &e)
    {
        if(e.type != SDL_KEYDOWN && e.type != SDL_KEYUP)
        {
            LogMsg(ERROR, "passed wrong event type to player");
            return;
        }

        if(e.key.repeat != 0)
            return;

        // false if key up, true if down
        bool state = SDL_KEYUP - e.type == 1;

        switch(e.key.keysym.sym)
        {
            case SDLK_w:
                State.forward = state;
                break;
            case SDLK_s:
                State.backward = state;
                break;
            case SDLK_a:
                State.left = state;
                break;
            case SDLK_d:
                State.right = state;
                break;
            case SDLK_LEFT:
                State.lookleft = state;
                break;
            case SDLK_RIGHT:
                State.lookright = state;
                break;
        }
    }

    void Player::Update(float dtMs)
    {
        vec2d newAcc(0.0f);
        vec2d viewDir = vec2d::AngToVec(mViewAngle);

        if(State.forward)
            newAcc += viewDir;
        if(State.backward)
            newAcc -= viewDir;

        if(State.left)
            newAcc -= viewDir.GetPerpendicular();
        if(State.right)
            newAcc += viewDir.GetPerpendicular();

        if(State.lookleft)
            mViewAngle += mRotateSpeed * dtMs;
        if(State.lookright)
            mViewAngle -= mRotateSpeed * dtMs;
        
        // if player not moving apply friction
        if(!newAcc.GetMagnitude())
            newAcc = mVel * -0.05f;
        else
            newAcc.SetMagnitude(0.0005f);

        // update velocity
        mVel += newAcc * dtMs;

        // clamp velocity magnitude
        if(mVel.GetMagnitude() > mMaxMoveSpeed)
            mVel.SetMagnitude(mMaxMoveSpeed);
        else if(mVel.GetMagnitude() < 0.000001f)
            mVel = vec2d(0.0f);

        vec2d newPos = mPos + (mVel * dtMs);

        ResolveWallCollision(newPos);

        mCurrentGun.Update(dtMs);
    }

    void Player::Draw(Renderer& render)
    {
        mCurrentGun.Draw(render);
    }

}