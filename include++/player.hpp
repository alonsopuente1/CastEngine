#pragma once

#include "vec2d.hpp"
#include "entity.hpp"
#include "camera.hpp"
#include "gun.hpp"

#include <SDL2/SDL.h>

namespace CastEngine
{
    class Map;
    class Player : public Entity
    {

    private:

        float mMaxMoveSpeed;
        
        // rotate speed of key presses
        float mRotateSpeed;

        float mViewAngle = 0.0f;

        /// @brief stores the state of the player
        struct 
        {
            bool forward : 1;
            bool backward : 1;
            bool left : 1;
            bool right : 1;
            bool lookleft : 1;
            bool lookright : 1;
            bool shooting : 1;
        } State{};

        Gun mCurrentGun;

    public:

        Player(IWorld& world, Renderer& render);

        inline void SetRotateSpeed(const float& newSpeed) { mRotateSpeed = newSpeed; }
        inline void SetMaxSpeed(const float& maxSpeed) { mMaxMoveSpeed = maxSpeed; }

        inline vec2d GetDir() const { return vec2d::AngToVec(mViewAngle); }
        
        inline bool TryShoot() { return mCurrentGun.TryShoot(); }
        

        void HandleKeyInput(const SDL_Event& e);
        void Update(float dtMs) override;
        void Draw(Renderer& render) override;
    };
};