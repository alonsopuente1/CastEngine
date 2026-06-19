#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "castengine/vec2d.hpp"

namespace CastEngine
{

    class Map
    {

    private:
        
        // flat 2d array
        std::vector<int> mMapData;

        int mWidth;
        int mHeight;

        // stores file path to map on successful load
        std::string mFilePath;

        bool FillMapData(std::fstream& file);

    public:

        /// @brief stores extra information from the map
        struct LoadArgs
        {
            // start position of player in the map
            vec2d startPos;

            float maxSpeed;
            float rotateSpeed;

            // true if map loaded successfully
            bool success;

        };

        
        /// @return -1 if index out of bounds
        int operator[] (int i) const;

        bool LoadMap(const std::string& pFilePath, LoadArgs& args);

        inline int GetWidth() const { return mWidth; }
        inline int GetHeight() const { return mHeight; }


        struct RayCastDesc
        {
            enum
            {
                RAY_HIT_NONE = -1,
                RAY_HIT_VERTICAL = 0,
                RAY_HIT_HORIZONTAL = 1
            } side;

            vec2d pointOfCollision;
            float perpWallDist;
        };

        /// @brief performs a raycast from given point and direction
        /// @param origin origin of ray
        /// @param dir direction of ray
        /// @param desc collision description of raycast
        /// @return the value of the wall that the ray hit. -1 if ray didnt hit any wall
        int WallRayCast(const vec2d& origin, vec2d dir, RayCastDesc& desc) const;

        Map();

        void Destroy();

        void print();

    };

};