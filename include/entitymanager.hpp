#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <utility>

#include "vec2d.hpp"

namespace CastEngine
{

    class Entity;
    class Map;
    class Renderer;
    class EntityManager
    {

    private:

        struct Collision
        {
            Entity* a;
            Entity* b;
        };

        std::vector<std::unique_ptr<Entity>> mEntities;

        /// @brief loops over the entity list and detects all collision between entities
        /// @return vector of collision descriptions
        std::vector<Collision> DetectCollisions();

        /// @brief compares two entities and returns true if they are colliding
        /// @param ent1 first entity
        /// @param ent2 second entity
        /// @return true if colliding, false if not
        bool CheckCollision(Entity& ent1, Entity& ent2);

    public:

        EntityManager() {}

        const std::vector<std::unique_ptr<Entity>>& GetEntities() const; 
        void UpdateEntities(float dtMs);
        void DrawEntities(Renderer& render);

        inline Entity* PushEntity(std::unique_ptr<Entity>&& pEntity) { mEntities.push_back(std::move(pEntity)); return mEntities.back().get(); }

        std::pair<Entity*, float> RayCast(vec2d origin, vec2d dir, float maxDist);

        void RemoveIf(std::function<bool(Entity*)> pFunc);
    };

};