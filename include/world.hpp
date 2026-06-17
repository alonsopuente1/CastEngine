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
    class Texture;
    class IWorld
    {

    public:

        virtual ~IWorld() = default;

        /// @brief spawns an entity into the entity manager of the derived scene
        /// @param ptr pointer to the created entity to move into entity manager
        virtual void SpawnEntity(std::unique_ptr<Entity>&& ptr) = 0;
        
        /// @return returns map of the derived scene
        virtual Map& GetMap() = 0;

        /// @brief function should return true if the position given is a wall in the map
        /// @param pos vector position in the map
        /// @return true if wall, false if not
        virtual bool IsWall(vec2d& pos) = 0;
        
        /// @brief this function should return true when the position given is
        /// a wall
        /// @param x x position in the map
        /// @param y y position in the map
        /// @return true if it is a wall, false if not
        virtual bool IsWall(int x, int y) = 0;

        virtual vec2d GetPlayerPos() const = 0;
        virtual vec2d GetPlayerDir() const = 0;
    };

};