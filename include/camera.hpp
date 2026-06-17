#pragma once

#include "vec2d.hpp"

namespace CastEngine
{
    class Player;
    class Camera
    {
    
    private:

        vec2d mPos{};
        // angle stored in radians
        vec2d mDir{};
        float mFov = M_PI / 2.f;

    public:

        Camera() = default;

        void Follow(Player& player);

        inline void SetDir(const vec2d& vec) { mDir = vec; }
        inline vec2d GetDir() const { return mDir; }

        inline const vec2d& GetPos() const { return mPos; }
        inline void SetPos(const vec2d& newPos) { mPos = newPos; }
        inline void Rotate(float ang) { mDir.Rotate(ang); }

        inline float GetFOV() const { return mFov; }
        inline void SetFOV(float newFOV) { mFov = newFOV; }
    };

};