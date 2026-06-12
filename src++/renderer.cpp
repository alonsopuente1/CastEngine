#include "renderer.hpp"

#include "window.hpp"
#include "player.hpp"
#include "map.hpp"
#include "camera.hpp"

#include "logger.hpp"

#include <cfloat>

void CastEngine::Renderer::ResetDepthBuffer()
{
    depthBuffer.resize(mWindow.GetWidth());
    for(float& f : depthBuffer)
    {
        f = FLT_MAX;
    }
}

CastEngine::Renderer::Renderer(Window &window) : mWindow(window)
{
    if(!mWindow.IsInitialised())
    {
        LogMsg(ERROR, "passed uninitialised window to Renderer constructor\n");
        exit(-1);
    }
    ResetDepthBuffer();
}

CastEngine::Renderer::~Renderer()
{
    Destroy();
}

bool CastEngine::Renderer::operator==(const Renderer &other)
{
    return mWindow == other.mWindow;
}

bool CastEngine::Renderer::operator!=(const Renderer &other)
{
    return !(*this == other);
}

bool CastEngine::Renderer::RenderTexture(const Texture &tex, SDL_Rect src, SDL_Rect dst)
{
    if(SDL_RenderCopy(mWindow.GetRenderer(), tex.GetTexture(), &src, &dst) < 0)
    {
        LogMsgf(ERROR, "failed to render texture. SDL_ERROR: %s", SDL_GetError());
        return false;
    }    
    return true;
}

bool CastEngine::Renderer::RenderCircle(SDL_Point centre, float radius, SDL_Color pColour)
{
    SDL_SetRenderDrawColor(mWindow.GetRenderer(), pColour.r, pColour.g, pColour.b, pColour.a);

    // form vertices for the circle
    
    const int numPoints = 20;
    const float angle = (M_PI * 2) / numPoints;

    for(int i = 0; i < numPoints; i++)
    {
        int x1 = cosf(angle * i) * radius + centre.x; 
        int y1 = sinf(angle * i) * radius + centre.y;

        int x2 = cosf(angle * (i + 1)) * radius + centre.x;
        int y2 = sinf(angle * (i + 1)) * radius + centre.y;

        if(SDL_RenderDrawLine(mWindow.GetRenderer(), x1, y1, x2, y2) < 0)
        {
            LogMsgf(ERROR, "failed to render line for cirlce. SDL_ERROR: %s", SDL_GetError());
            return false;
        }
    }

    return true;
}

bool CastEngine::Renderer::RenderFillCircle(SDL_Point centre, float radius, SDL_Color pColour)
{
    // number of triangles to use for the circle
    // more triangles = better definition of circle
    const int numTriangles = 20;

    int numVertices = numTriangles * 3;

    std::vector<SDL_Vertex> vertices;
    vertices.resize(static_cast<size_t>(numVertices));
    float triangleAngle = (M_PI * 2) / static_cast<float>(numTriangles);
    
    for(int i = 0; i < numVertices; i+= 3)
    {
        int j = i / 3;

        SDL_Vertex* centreVertex = &vertices[i];
        SDL_Vertex* currentVertex = &vertices[i + 1];
        SDL_Vertex* finalVertex = &vertices[i + 2];

        centreVertex->color = pColour;
        currentVertex->color = pColour;
        finalVertex->color = pColour;

        centreVertex->position.x = centre.x;
        centreVertex->position.y = centre.y; 

        currentVertex->position.x = cosf(triangleAngle * j) * radius + centre.x;
        currentVertex->position.y = sinf(triangleAngle * j) * radius + centre.y;

        finalVertex->position.x = cosf(triangleAngle * (j + 1)) * radius + centre.x;
        finalVertex->position.y = sinf(triangleAngle * (j + 1)) * radius + centre.y;
        
    }

    if(SDL_RenderGeometry(mWindow.GetRenderer(), NULL, vertices.data(), numVertices, NULL, 0) < 0)
    {
        LogMsg(ERROR, "failed to render circle, operation unsupported");
        return false;
    }

    return true;
}

void CastEngine::Renderer::RenderSprite(Texture* tex, vec2d target)
{
    vec2d camToTarget = target - mCurrentCamera->GetPos();
    vec2d dir = mCurrentCamera->GetDir().Normalised();
    vec2d plane = dir.GetPerpendicular();

    plane.SetMagnitude(tanf(mCurrentCamera->GetFOV() / 2.0f));

    float invDet = 1.0f / (plane.x * dir.y - dir.x * plane.y);

    vec2d transform = {
        invDet * (dir.y * camToTarget.x - dir.x * camToTarget.y),
        invDet * (-plane.y * camToTarget.x + plane.x * camToTarget.y)
    };

    int spriteScreenX = static_cast<int>((mWindow.GetWidth() / 2.f) * (1.f + transform.x / transform.y));

    int spriteHeight = abs(static_cast<int>(mWindow.GetHeight() / transform.y));

    int spriteWidth = (tex->GetWidth() / tex->GetHeight()) * spriteHeight;

    int drawStartX = -spriteWidth / 2.f + spriteScreenX;
    int drawEndX = spriteWidth / 2.f + spriteScreenX;

    SDL_SetRenderDrawBlendMode(mWindow.GetRenderer(), SDL_BLENDMODE_BLEND);
    for(int i = drawStartX; i < drawEndX; i++)
    {
        int texX = static_cast<int>(static_cast<float>(i - drawStartX) / static_cast<float>(drawEndX - drawStartX) * tex->GetWidth());

        SDL_Rect src = {texX, 0, 1, static_cast<int>(tex->GetHeight())};
        SDL_Rect dst = {i, -spriteHeight / 2 + mWindow.GetHeight() / 2, 1, spriteHeight};

        if(transform.y > 0 && i > 0 && i < mWindow.GetWidth() && transform.y < depthBuffer[i])
        {
            depthBuffer[i] = transform.y;
            SDL_RenderCopy(mWindow.GetRenderer(), tex->GetTexture(), &src, &dst);
        }
    }
    SDL_SetRenderDrawBlendMode(mWindow.GetRenderer(), SDL_BLENDMODE_NONE);
}

void CastEngine::Renderer::RenderCameraView(const Map& pMap)
{
    vec2d dir = mCurrentCamera->GetDir();

    vec2d plane = dir.GetPerpendicular();
    plane.SetMagnitude(tanf(mCurrentCamera->GetFOV() / 2.0f));

    const CastEngine::Texture* textures[10] = { 0 };

    vec2d ppos = mCurrentCamera->GetPos();

    int numTextures = static_cast<int>(sizeof(textures) / sizeof(textures[0]));
    for(int i = 0; i < numTextures; i++)
    {
        textures[i] = texBank[i];
    }

    for(int x = 0; x < mWindow.GetWidth(); x++)
    {
        float cameraX = (float)x / (float)mWindow.GetWidth() * 2.0f - 1.0f;
        vec2d rayDir = dir + (plane * cameraX);
        int mapX = (int)ppos.x;
        int mapY = (int)ppos.y;

        vec2d sideDist;

        vec2d deltaDist;
        deltaDist.x = (rayDir.x == 0) ? FLT_MAX : fabs(1 / rayDir.x);
        deltaDist.y = (rayDir.y == 0) ? FLT_MAX : fabs(1 / rayDir.y);
        float perpWallDist;

        int stepX;
        int stepY;

        int hit = 0;
        int side;

        if(rayDir.x < 0)
        {
            stepX = -1;
            sideDist.x = (ppos.x - mapX) * deltaDist.x;
        }
        else
        {
            stepX = 1;
            sideDist.x = (mapX + 1.0f - ppos.x) * deltaDist.x;
        }

        if(rayDir.y < 0)
        {
            stepY = -1;
            sideDist.y = (ppos.y - mapY) * deltaDist.y;
        }
        else
        {
            stepY = 1;
            sideDist.y = (mapY + 1.0f - ppos.y) * deltaDist.y;
        }

        while(hit == 0)
        {
            if(sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapX += stepX;
                side = 0;
            }

            else
            {
                sideDist.y += deltaDist.y;
                mapY += stepY;
                side = 1;
            }
            if(mapX < 0 || mapX >= pMap.GetWidth() || mapY < 0 || mapY >= pMap.GetHeight())
                break;

            if(pMap[mapY * pMap.GetWidth() + mapX] > 0) hit = 1;
        }



        if(side == 0)   perpWallDist = (sideDist.x - deltaDist.x);
        else            perpWallDist = (sideDist.y - deltaDist.y);

        if(x < static_cast<int>(depthBuffer.size()) && perpWallDist >= depthBuffer[x])
            continue;

        depthBuffer[x] = perpWallDist;

        int lineHeight = (int)((float)mWindow.GetHeight() / perpWallDist);

        int drawStart = -lineHeight / 2 + mWindow.GetHeight() / 2;

        int texNum = pMap[mapY * pMap.GetHeight() + mapX] - 1;

        if(texNum < 0)
            continue;

        const Texture* wallTexture = textures[texNum];

        double wallX;

        if(side == 0)
            wallX = ppos.y + perpWallDist * rayDir.y;
        else
            wallX = ppos.x + perpWallDist * rayDir.x;


        wallX -= floor(wallX);

        int texX = static_cast<int>(wallX * static_cast<double>(wallTexture->GetWidth()));
        if(side == 0 && rayDir.x > 0) texX = wallTexture->GetWidth() - texX - 1;
        if(side == 1 && rayDir.y < 0) texX = wallTexture->GetWidth() - texX - 1;

        SDL_Rect src = {texX, 0, 1, static_cast<int>(wallTexture->GetHeight())};
        SDL_Rect dst = {x, drawStart, 1, lineHeight};

        RenderTexture(*wallTexture, src, dst);
    }
}

void CastEngine::Renderer::RenderCeilingAndFloor(SDL_Colour topColour, SDL_Colour bottomColour)
{
    SDL_Rect dest = {0, 0, mWindow.GetWidth(), mWindow.GetHeight() / 2};

    SDL_SetRenderDrawColor(mWindow.GetRenderer(), topColour.r, topColour.g, topColour.b, topColour.a);
    SDL_RenderFillRect(mWindow.GetRenderer(), &dest);

    dest.y += mWindow.GetHeight() / 2;
    SDL_SetRenderDrawColor(mWindow.GetRenderer(), bottomColour.r, bottomColour.g, bottomColour.b, bottomColour.a);
    SDL_RenderFillRect(mWindow.GetRenderer(), &dest);

}

void CastEngine::Renderer::ClearScreen(SDL_Color &colour)
{
    SDL_Renderer* render = mWindow.GetRenderer();
    ResetDepthBuffer();

    if(SDL_SetRenderDrawColor(render, colour.r, colour.g, colour.b, colour.a) < 0)
    {
        LogMsgf(ERROR, "failed to clear screen. SDL_ERROR: %s", SDL_GetError());
        return;
    }
    
    if(SDL_RenderClear(render) < 0)
        LogMsgf(ERROR, "failed to clear screen. SDL_ERROR: %s", SDL_GetError());

}

void CastEngine::Renderer::Present()
{
    SDL_RenderPresent(mWindow.GetRenderer());
}

void CastEngine::Renderer::Destroy()
{
    texBank.FreeAll();
    ResetDepthBuffer();
}
