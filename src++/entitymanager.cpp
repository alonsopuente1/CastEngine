#include "entitymanager.hpp"

#include "map.hpp"
#include "entity.hpp"
#include "enemy.hpp"

#include "vec2d.hpp"

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

    void EntityManager::RemoveIf(std::function<bool(Entity*)> pFunc)
    {
        for(std::unique_ptr<Entity>& ent : mEntities)
        {
            if(pFunc(ent.get()))
            {

            }
        }
    }
};
