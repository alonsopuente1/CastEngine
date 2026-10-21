#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>
#include <string>
#include <vector>

/* texture.hpp



*/

namespace CastEngine
{

    // forward decleration
    class Window;

    /// @brief texture class for handling textures in the engine
    /// MUST be created with a window
    class Texture 
    {

    public:

        struct CPUTexture
        {
            SDL_PixelFormatEnum mPixelDataFormat = static_cast<SDL_PixelFormatEnum>(0);
            std::vector<uint32_t> mPixelData = {};
            int mPixelDataWidth = 0, mPixelDataHeight = 0;
        };

    private:
        bool mSDLTexLocked = false;
        
        SDL_Texture*    mSDLTex = nullptr;
        // name of texture. can be used as unique id or not.
        // usually holds the file path from the last image it loaded
        std::string     mName;
        
        Window&         mWindow;
        
        CPUTexture      mCPUData;

    public:
        
        union Colour
        {
            uint32_t val;
            SDL_PixelFormatEnum format;
        };

        /// @brief textures have to be attached to a window
        ///     the textures only work for that window
        /// @param window window to attach to
        Texture(Window& window);
        Texture(Window& window, SDL_Texture* sdlTex);
        ~Texture();

        /// @brief textures have to be attached to a window
        ///     the textures only work for that window
        /// @param window window to attach to
        /// @param file file path to the image to load
        Texture(Window& window, const std::string& file);
        
        /// @brief textures have to be attached to a window.
        /// @brief the textures only work for the window attached  
        /// @param window window to attach to
        /// @param name name to give to texture
        /// @param width width of the texture 
        Texture(Window& window, const std::string& name, int width, int height);

        // move constructors
        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& tex) noexcept;
        

        /// @brief getter method for width
        /// @return width of texture (0 if no texture loaded)
        uint32_t GetWidth() const;
        /// @brief getter method for height
        /// @return height of texture (0 if no texture loaded)
        uint32_t GetHeight() const;

        /// @brief sets the name of texture
        /// @param newName name to set to the texture
        /// @return 
        void SetTextureName(const std::string& newName);
        const std::string& GetTextureName() const;

        /// @brief destroys the texture but keeps window attached
        void Destroy();
        
        /// @brief returns if texture exists or not
        /// @return true if does exist, false if not
        bool IsInitialised() const;

        
        /// @brief getter method for internal SDL texture
        /// @return NULL if not initialised
        SDL_Texture* GetTexture() const;
        
        CPUTexture& GetCPUTexture() { return mCPUData; }

        /// @brief loads a texture from image file. replaces any existing texture
        /// @param filePath path to image file
        bool LoadImage(const std::string& filePath);
        
        /// @brief destroys current texture and replaces with blank texture
        /// @param pWidth width of new texture
        /// @param pHeight height of new texture;
        /// @return true if successful, false if not
        bool CreateBlankTexture(const std::string& pName, int pWidth, int pHeight, SDL_TextureAccess access = SDL_TEXTUREACCESS_TARGET);

        bool LockTexture(const SDL_Rect *rect, void **pixels, int *pitch);
        void UnlockTexture();

        /// @brief copies the pixel data into internal CPU data
        void SyncTextureToCPU();

        /// @brief takes a snapshot of the GPU texture and copies it into
        /// @brief CPU storage. format will match the GPU texture format
        /// @warning this method is expensive, use wisely
        bool TextureCPUSnapshot();

        /// @warning function expects for LockSurface to have been called before this 
        Colour GetColour(int x, int y) const;
        /// @brief sets COLOUR to position X Y in surface. call SyncTextureToSurface to update GPU texture
        /// @warning function expects for LockSurface to have been called before this
        bool SetColour(Colour colour, int x, int y);

        /// @brief converts the format of the values in 'pixels' to chosen format. assumes 'pixels
        /// is a tightly packed array of pixels with no padding
        /// @param pixels array of pixel values to convert
        /// @param srcFormat format of 'pixels'
        /// @param dstFormat new format to convert 'pixels'
        /// @param pitch size of one row of 'pixels' in bytes
        /// @param width width of pixels
        /// @param height height of pixels
        /// @return true on success, false on failure
        static bool ConvertPixelFormat(CPUTexture& tex, SDL_PixelFormatEnum srcFormat, SDL_PixelFormatEnum dstFormat, int pitch, int width, int height);
        inline SDL_PixelFormatEnum GetPixelFormat() const { return mCPUData.mPixelDataFormat; }
        inline bool IsCPUTextureAvailable() const { return !mCPUData.mPixelData.empty(); }
        
        Window& GetAttachedWindow() const;
        void SetAttachedWindow(Window& pWindow);

        
    };
};