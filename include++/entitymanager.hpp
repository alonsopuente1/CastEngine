#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace CastEngine
{

    class Entity;
    class Map;
    class EntityManager
    {

    private:

        std::vector<std::unique_ptr<Entity>> mEntities;

    public:

        EntityManager() {}

        void UpdateEntities(float dtMs);

        inline Entity* PushEntity(std::unique_ptr<Entity>&& pEntity) { mEntities.push_back(std::move(pEntity)); return mEntities.back().get(); }

        void RemoveIf(std::function<bool(Entity*)> pFunc);
    };

};