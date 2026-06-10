#pragma once

#include <memory>

#include "vec2d.hpp"

/* world.hpp

    interface for a world which a scene should implement for
    entities to be able to access data without dependance on
    main game/scene

    any scenes that inherit the IWorld interface should override
    the virtual functions so that entities can access needed data

*/

namespace CastEngine
{

    class Entity;
    class Map;
    class IWorld
    {

    public:

        virtual ~IWorld() = default;

        virtual void SpawnEntity(std::unique_ptr<Entity> ptr) = 0;
        virtual Map& GetMap() = 0;

        virtual bool IsWall(vec2d& pos) = 0;
        virtual bool IsWall(int x, int y) = 0;

    };

};