#pragma once

#include <memory>

#include "vec2d.hpp"

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