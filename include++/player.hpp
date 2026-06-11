#pragma once

#include "vec2d.hpp"
#include "entity.hpp"
#include <SDL2/SDL.h>

namespace CastEngine
{
    class Map;
    class Player : public Entity
    {

    private:
        
        /// @brief stored in radians
        float mViewAng;

        float mMaxMoveSpeed;
        
        /// @brief in radians
        const float mFov = M_PI / 2.f;

        float mRotateSpeed;

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

        
        
        void Rotate(float ang);

    public:

        using Entity::Entity;

        inline void SetRotateSpeed(const float& newSpeed) { mRotateSpeed = newSpeed; }
        inline void SetMaxSpeed(const float& maxSpeed) { mMaxMoveSpeed = maxSpeed; }

        inline float GetViewAng() const { return mViewAng; }
        inline const float GetFOV() const { return mFov; }

        void HandleKeyInput(const SDL_Event& e);
        void Update(float dtMs) override;
        void Draw() override;
    };
};