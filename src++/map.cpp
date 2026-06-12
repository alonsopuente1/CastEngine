#include "map.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "vec2d.hpp"
#include "logger.hpp"

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
            return 0;

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