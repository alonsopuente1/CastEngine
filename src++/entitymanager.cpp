#include "entitymanager.hpp"

#include "map.hpp"
#include "entity.hpp"

namespace CastEngine
{
    void EntityManager::UpdateEntities(float dtMs)
    {
    
        for(std::unique_ptr<Entity>& ent : mEntities)
        {
            ent->Update(dtMs);
        }
    
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
