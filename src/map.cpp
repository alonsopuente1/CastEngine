#include "castengine/map.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cfloat>

#include "castengine/vec2d.hpp"
#include "castengine/logger.hpp"

namespace CastEngine
{
    bool Map::FillMapData(std::fstream &file)
    {
        int y = 0;
        std::string currentLine = "";

        if(!mWidth || !mHeight)
        {
            SetErrorf("invalid width or height (%ix%i)", mWidth, mHeight);
            return false;
        }

        while(std::getline(file, currentLine))
        {
            char* sanitised = strtok(currentLine.data(), " \n\r\t");
            if(!sanitised)
            {
                LogMsg(ERROR, "sanitised has nothing");
                break;
            }

            if((int)strlen(sanitised) != mWidth)
            {
                SetErrorf("Map width doesnt match dimensions set in map file %s\n", mFilePath.c_str());
                return false;
            }

            for(int x = 0; x < mWidth; x++)
            {
                if(sanitised[x] < '0' || sanitised[x] > '9')
                {
                    SetErrorf("Invalid character '%c' in map data at row %d column %d in map file %s\n", currentLine[x], y, x, mFilePath.c_str());
                    return false;
                }
                mMapData[y * mWidth + x] = sanitised[x] - '0';
            }

            y++;
        }

        if(y != mHeight)
        {
            SetErrorf("Rows dont match in map at '%s'\n", mFilePath.c_str());
            return false;
        }

        return true;
    }

    int Map::operator[] (int i) const
    {
        if(i < 0 || i > static_cast<int>(mMapData.size()) - 1)
            return -1;

        return mMapData[i];
    }

    bool Map::LoadMap(const std::string& pFilePath, Map::LoadArgs& args)
    {
        std::fstream file(pFilePath);
        args.success = false;

        if(!file.is_open())
        {
            LogMsgf(ERROR, "failed to open file at path '%s'", pFilePath.c_str());
            return false;
        }

        mFilePath = pFilePath;

        int lineNum = 0;
        std::string currentLine = "";
        while(std::getline(file, currentLine))
        {
            lineNum++;
            std::stringstream stream;
            stream << currentLine;

            std::string firstToken = "";
            stream >> firstToken;

            if(firstToken.empty() || firstToken[0] == '#' || firstToken[0] == '\n' || firstToken[0] == '\r')
                continue;
            
            if(firstToken == "mapdim")
            {
                stream >> mWidth >> mHeight;

                if(mWidth <= 0 || mHeight <= 0)
                {
                    LogMsgf(ERROR, "Invalid map dimensions in map file '%s' on line %i", pFilePath.c_str(), lineNum);
                    Destroy();
                    return false;
                }

                mMapData.resize(mWidth * mHeight);

                continue;
            }

            if(firstToken == "mapstart")
            {
                for(int y = 0; y < mHeight; y++)
                {
                    std::getline(file, currentLine);
                    lineNum++;
                    if(static_cast<int>(currentLine.length()) != mWidth)
                    {
                        LogMsgf(ERROR, "map width doesnt match set dimensions. in file '%s' on line %i", pFilePath.c_str(), lineNum);
                        return false;
                    }

                    for(int x = 0; x < mWidth; x++)
                    {
                        if(currentLine[x] < '0' || currentLine[x] > '9')
                        {
                            LogMsgf(ERROR, "invalid character '%c' on line %i in map data. in file '%s'", currentLine[x], lineNum, pFilePath.c_str());
                            return false;
                        }

                        mMapData[y * mWidth + x] = currentLine[x] - '0';
                    }
                }

                continue;
            }

            if(firstToken == "playerstart")
            {
                stream >> args.startPos.x >> args.startPos.y;
                continue;
            }

            if(firstToken == "playermaxspeed")
            {
                stream >> args.maxSpeed;
                continue;
            }

            if(firstToken == "playerrotatespeed")
            {
                stream >> args.rotateSpeed;
                continue;
            }

            // if execution reaches here, none of the token checks have 
            // been triggered therefore its an unrecognised token
            LogMsgf(ERROR, "unrecognised token in map file on line %i. token: '%s'", lineNum, firstToken.c_str());
            return false;
        }

        file.close();
        return true;
    }

    int Map::WallRayCast(const vec2d& origin, vec2d dir, RayCastDesc &desc) const
    {
        int mapX = static_cast<int>(origin.x);
        int mapY = static_cast<int>(origin.y);

        vec2d sideDist;
        vec2d deltaDist;
        // dir.Normalise();

        deltaDist.x = (dir.x == 0) ? FLT_MAX : fabs(1.f / dir.x);
        deltaDist.y = (dir.y == 0) ? FLT_MAX : fabs(1.f / dir.y);
        
        int wallVal = 0;

        int stepX = 0;
        int stepY = 0;


        desc.side = RayCastDesc::RAY_HIT_NONE;

        if(dir.x < 0)
        {
            stepX = -1;
            sideDist.x = (origin.x - mapX) * deltaDist.x;
        }
        else
        {
            stepX = 1;
            sideDist.x = (mapX + 1.0f - origin.x) * deltaDist.x;
        }

        if(dir.y < 0)
        {
            stepY = -1;
            sideDist.y = (origin.y - mapY) * deltaDist.y;
        }
        else
        {
            stepY = 1;
            sideDist.y = (mapY + 1.0f - origin.y) * deltaDist.y;
        }

        while(true)
        {
            if(sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapX += stepX;
                desc.side = RayCastDesc::RAY_HIT_VERTICAL;
            }
            else
            {
                sideDist.y += deltaDist.y;
                mapY += stepY;
                desc.side = RayCastDesc::RAY_HIT_HORIZONTAL;
            }

            if(mapX < 0 || mapX >= mWidth || mapY < 0 || mapY >= mHeight)
                wallVal = -1;
            else
                wallVal = (*this)[mapY * mWidth + mapX];

            if(wallVal > 0) 
            {
                if(desc.side == RayCastDesc::RAY_HIT_HORIZONTAL)
                    desc.perpWallDist = sideDist.y - deltaDist.y;
                else
                    desc.perpWallDist = sideDist.x - deltaDist.x;
                return wallVal;
            }
            // ray ended up out of bounds of map cells 
            else if(wallVal < 0)
            {
                desc.side = RayCastDesc::RAY_HIT_NONE;
                desc.pointOfCollision = vec2d(0.f);
                return -1;
            }
        }

        // execution should never reach here
    }

    Map::Map() : mFilePath("")
    {

    }

    void Map::Destroy()
    {
        mMapData.clear();

        mWidth = 0;
        mHeight = 0;

        mFilePath = "";
    }

    void Map::print()
    {
        for(int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                std::cout << mMapData[y * mWidth + x];
            }
            std::cout << '\n';
        }
    }
};