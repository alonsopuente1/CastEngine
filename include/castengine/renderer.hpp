#pragma once

#include "castengine/texturebank.hpp"
#include "castengine/vec2d.hpp"

#include <vector>

namespace CastEngine
{
  
    class Window;
    class Player;
    class Map;
    class Gun;
    class Camera;
    class EntityManager;

    /// @brief a renderer for rendering player views, entities and more
    /// should be created alongside a Window object
    class Renderer
    {

    private:
        
        Window& mWindow;

        const Camera* mCurrentCamera = nullptr;
    
        // pixels formatted as RGBA8888
        std::vector<uint32_t> mPixelBuffer;
        SDL_Texture* mScreenTex = nullptr;

        int mBufferWidth = 0;
        int mBufferHeight = 0;

        inline void SetPixel(int x, int y, uint32_t colour)
        {
            if(x < 0 || x >= mBufferWidth || y < 0 || y >= mBufferHeight)
                return;

            mPixelBuffer[static_cast<size_t>(y * mBufferWidth + x)] = colour;
        }

        bool InitScreenBuffer(int w, int h);

    public:
        
        Renderer(Window& window);
        ~Renderer();
        
        std::vector<float> depthBuffer;
        TextureBank texBank;
        
        void ResetDepthBuffer();
        inline void SetCamera(const Camera& cam) { mCurrentCamera = &cam; }

        inline Window& GetWindow() { return mWindow; }

        bool operator==(const Renderer& other);
        bool operator!=(const Renderer& other);

        bool RenderTexture(const Texture& tex, SDL_Rect src, SDL_Rect dst);

        /// @brief renders a circle to the screen
        bool RenderCircle(SDL_Point centre, float radius, SDL_Color pColour);
        /// @brief renders a filled circles to the screen
        bool RenderFillCircle(SDL_Point centre, float radius, SDL_Color pColour);

        /// @brief renders a sprite from the perspective of the currently set camera
        /// @warning SetCamera MUST be called before using this function, otherwise 
        /// program will seg fault.
        void RenderSprite(Texture* tex, vec2d target);

        /// @brief renders the given map from the perspective of the camera
        /// @param map map to render
        /// @warning SetCamera MUST be called before using this function, otherwise 
        /// program will seg fault.
        void RenderCameraView(const Map& map);

        void RenderSolidCeilingFloor(SDL_Colour topColour, SDL_Colour bottomColour);

        void ApplyLighting();
        void ClearScreen(SDL_Color& colour);
        void Present();

        void Destroy();

    };

};