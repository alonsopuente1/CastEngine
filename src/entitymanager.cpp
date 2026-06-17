#include "entitymanager.hpp"

#include "map.hpp"
#include "entity.hpp"
#include "enemy.hpp"

#include <algorithm>
#include <cfloat>

namespace CastEngine
{
    std::vector<EntityManager::Collision> EntityManager::DetectCollisions()
    {
        std::vector<Collision> output;

        int numEnts = mEntities.size();

        for(int i = 0; i < numEnts; i++)
        {
            for(int j = i + 1; j < numEnts; j++)
            {
                if(CheckCollision(*mEntities.at(i).get(), *mEntities.at(j).get()))
                    output.push_back({mEntities.at(i).get(), mEntities.at(j).get()});
            }
        }

        return output;
    }

    bool EntityManager::CheckCollision(Entity &ent1, Entity &ent2)
    {
        return ent1.GetRadius() + ent2.GetRadius() > (ent1.GetPos() - ent2.GetPos()).GetMagnitude();
    }

    const std::vector<std::unique_ptr<Entity>> &EntityManager::GetEntities() const
    {
        return mEntities;
    }

    void EntityManager::UpdateEntities(float dtMs)
    {
    
        // update the entities state, resolves wall collisions as well
        for(std::unique_ptr<Entity>& ent : mEntities)
        {
            ent->Update(dtMs);
        }
    
        // now resolve resolve collision with other entities
        std::vector<Collision> collisions = DetectCollisions();

        if(collisions.empty())
            return;
        
        for(Collision& col : collisions)
        {
            col.a->OnCollision(*col.b);
            col.b->OnCollision(*col.a);
        }
        
    }

    void EntityManager::DrawEntities(Renderer& render)
    {
        for(std::unique_ptr<Entity>& ent : mEntities)
            ent.get()->Draw(render);   
    }

    std::pair<Entity*, float> EntityManager::RayCast(vec2d origin, vec2d dir, float maxDist)
    {
        dir.Normalise();

        Entity* best = nullptr;
        float bestDist = FLT_MAX;

        const float hitRadius = 0.6f;

        for(const std::unique_ptr<Entity>& ent : mEntities)
        {
            Entity* rawEnt = ent.get();
            if(!rawEnt->IsAlive())
                continue;
            
            vec2d toEnt = rawEnt->GetPos() - origin;

            float t = toEnt.DotProduct(dir);

            if(t <= 0.0f)
                continue;

            vec2d closestPoint = origin + (dir * t);
            float perpDist = (closestPoint - rawEnt->GetPos()).GetMagnitude();

            if(perpDist <= hitRadius + rawEnt->GetRadius() && t < maxDist && t < bestDist)
            {
                bestDist = perpDist;
                best = rawEnt;
            }
        }

        if(best)
            return {best, bestDist};

        return {nullptr, 0.0f};
    }

    void EntityManager::RemoveIf(std::function<bool(Entity *)> pFunc)
    {
        
        

        mEntities.erase(std::remove_if(
            mEntities.begin(), mEntities.end(), 
            std::function<bool(std::unique_ptr<Entity>& ent)>(
                [](std::unique_ptr<Entity>& ent)
                {
                    return !ent->IsAlive();
                })
            ), mEntities.end()); //mEntities,
    }
};
