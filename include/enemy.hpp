#pragma once

#include "entity.hpp"

namespace CastEngine
{
    class Texture;
    class Enemy : public Entity
    {

    private:
        
        Texture* mTex;

        float mMaxSpeed;

    public:

        using Entity::Entity;

        /// @brief thinker function to implement simple AI
        void Think();
        void Update(float dtMs) override;
        void Draw(Renderer& render) override;

        inline void SetTexture(Texture* tex) { mTex = tex; }

        inline void SetMaxSpeed(float maxSpeed) { mMaxSpeed = maxSpeed; }
    };

}