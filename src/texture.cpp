#include "castengine/texture.hpp"

#include <SDL2/SDL_image.h>

#include <cstring>
#include <algorithm>

#include "castengine/logger.hpp"
#include "castengine/window.hpp"
#include "castengine/texture.hpp"

namespace CastEngine
{

    Texture::Texture(Window& window) : mSDLTex(NULL), mName(""), mWindow(window)
    {}

    Texture::Texture(Window &window, SDL_Texture *sdlTex) : mSDLTex(sdlTex), mName(""), mWindow(window)
    {
    }

    Texture::Texture(Window& window, const std::string& file) : mSDLTex(NULL), mName(""), mWindow(window)
    {
        LoadImage(file);
    }

    Texture::~Texture()
    {
        Destroy();
    }

    Texture::Texture(Texture &&other) noexcept : mSDLTex(other.mSDLTex), mName(other.mName), mWindow(other.mWindow), mCPUData(other.mCPUData)
    {
        other.mCPUData = CPUTexture();
        other.mSDLTex = nullptr;
        other.mName = "";
    }

    Texture &Texture::operator=(Texture &&tex) noexcept
    {
        if(this == &tex)
            return *this;

        Destroy();

        mSDLTex = tex.mSDLTex;
        tex.mSDLTex = NULL;

        mCPUData.mPixelData = std::move(tex.mCPUData.mPixelData);
        tex.mCPUData.mPixelData = {};
        
        mCPUData.mPixelDataFormat = tex.mCPUData.mPixelDataFormat;
        tex.mCPUData.mPixelDataFormat = static_cast<SDL_PixelFormatEnum>(0);
        
        mCPUData.mPixelDataWidth = tex.mCPUData.mPixelDataWidth;
        tex.mCPUData.mPixelDataWidth = 0;

        mCPUData.mPixelDataHeight = tex.mCPUData.mPixelDataHeight;
        tex.mCPUData.mPixelDataHeight = 0;

        return *this;
    }

    Texture::Texture(Window& window, const std::string& name, int width, int height) : mSDLTex(NULL), mName(""), mWindow(window)
    {
        CreateBlankTexture(name, width, height);
    }
    
    uint32_t Texture::GetWidth() const
    {
        int width;
        if(SDL_QueryTexture(mSDLTex, NULL, NULL, &width, NULL) < 0)
        {
            LogMsgf(ERROR, "failed to retrieve width of texture (%s)", mName.c_str());
            return 0;
        }
        return width;
    }
    uint32_t Texture::GetHeight() const
    {
        int height;
        if(SDL_QueryTexture(mSDLTex, NULL, NULL, NULL, &height) < 0)
        {
            LogMsgf(ERROR, "failed to retrieve height of texture (%s)", mName.c_str());
            return 0;
        }
        return height;
    }
    
    void Texture::Destroy()
    {
        if(mSDLTex)
            SDL_DestroyTexture(mSDLTex);
        mSDLTex = nullptr;
        mName = "";
    }
    
    bool Texture::IsInitialised() const
    {
        return mSDLTex != NULL;
    }

    void Texture::SetTextureName(const std::string& newName)
    {
        mName = newName;
    }

    const std::string &Texture::GetTextureName() const
    {
        return mName;
    }

    SDL_Texture *Texture::GetTexture() const
    {
        if(mSDLTexLocked)
        {
            LogMsg(WARN, "cannot get surface, surface is locked");
            return nullptr;
        }
        return mSDLTex;
    }

    bool Texture::LoadImage(const std::string &filePath)
    {
        SDL_Surface* newSurface = IMG_Load(filePath.c_str());
        if(!newSurface)
        {
            LogMsgf(ERROR, "failed to load image file (%s). SDL_ERROR: %s", filePath.c_str(), IMG_GetError());
            return false;
        }

        SDL_Texture* newTex = SDL_CreateTextureFromSurface(mWindow.GetRenderer(), newSurface);
        if(!newTex)
        {
            LogMsgf(ERROR, "failed to create texture from surface (%s). SDL_ERROR: %s", filePath.c_str(), SDL_GetError());
            SDL_FreeSurface(newSurface);
            return false;
        }

        // only destroy after checking texture was able to be created
        if(IsInitialised())
            Destroy();

        mSDLTex = newTex;

        SetTextureName(filePath);
        return true;
    }
    
    bool Texture::CreateBlankTexture(const std::string &pName, int pWidth, int pHeight, SDL_TextureAccess access)
    {
        if(!mWindow.IsInitialised())
        {
            LogMsg(ERROR, "attempting to create texture on uninitialised window");
            return false;
        }

        SDL_Surface* newSurface = SDL_CreateRGBSurfaceWithFormat(0, pWidth, pHeight, 32, SDL_PIXELFORMAT_RGBA8888);
        if(!newSurface)
        {
            LogMsgf(ERROR, "failed to create blank surface. SDL_ERROR: %s", SDL_GetError());
            return false;
        }

        // create new blank texture
        SDL_Texture* newTex = SDL_CreateTexture(mWindow.GetRenderer(), SDL_PIXELFORMAT_RGBA8888, access, pWidth, pHeight);
        if(newTex == NULL)
        {
            LogMsgf(ERROR, "failed to create blank texture. SDL_ERROR: %s", SDL_GetError());
            SDL_FreeSurface(newSurface);
            return false;
        }
        
        // only destroy after checking texture was able to be created
        if(IsInitialised())
            Destroy();

        mSDLTex = newTex;
        SetTextureName(pName);

        return true;
    }

    bool Texture::LockTexture(const SDL_Rect *rect, void **pixels, int *pitch)
    {
        if(mSDLTexLocked)
            return true;
        if(SDL_LockTexture(mSDLTex, rect, pixels, pitch) < 0)
        {
            LogMsgf(ERROR, "failed to lock texture. SDL_ERROR: %s", SDL_GetError());
            return false;
        }
        mSDLTexLocked = true;
        return true;
    }

    void Texture::UnlockTexture()
    {
        SDL_UnlockTexture(mSDLTex);
        mSDLTexLocked = false;
    }

    Texture::Colour Texture::GetColour(int x, int y) const
    {
        if(mCPUData.mPixelData.empty())
            return {};

        Texture::Colour output = {};

        int index = x + y * mCPUData.mPixelDataWidth;

        output.val = mCPUData.mPixelData[index];
        output.format = mCPUData.mPixelDataFormat;

        return output;
    }

    bool Texture::SetColour(Colour colour, int x, int y)
    {
        if(mCPUData.mPixelData.empty())
            return false;

        int index = x + mCPUData.mPixelDataWidth * y;

        if(mCPUData.mPixelDataFormat != colour.format)
        {
            LogMsg(ERROR, "input colour format doesnt match CPU data format");
            return false;
        }
        
        mCPUData.mPixelData[index] = colour.val;

        return true;
    }

    bool Texture::ConvertPixelFormat(CPUTexture& tex, SDL_PixelFormatEnum srcFormat, SDL_PixelFormatEnum dstFormat, int pitch, int width, int height)
    {
        if(tex.mPixelData.empty() || tex.mPixelData.size() != static_cast<size_t>(width * height))
        {
            return false;
        }

        std::vector<uint32_t> newPixels = {};
        newPixels.reserve(width * height);

        if(SDL_ConvertPixels(width, height, srcFormat, tex.mPixelData.data(), pitch, dstFormat, newPixels.data(), pitch) < 0)
        {
            return false;
        }

        tex.mPixelData = std::move(newPixels);
        tex.mPixelDataFormat = dstFormat;

        return true;
    }

    Window &Texture::GetAttachedWindow() const
    {
        return mWindow;
    }

    void Texture::SetAttachedWindow(Window& pWindow)
    {
        if(IsInitialised())
            LogMsgf(WARN, "setting new attached window without destroying old texture (%s)... destroying texture...", mName.c_str());

        Destroy();

        mWindow = pWindow;
    }

    void Texture::SyncTextureToCPU()
    {
        uint32_t* pixels;
        int pitch;

        if(!mSDLTexLocked)
            LockTexture(NULL, reinterpret_cast<void**>(&pixels), &pitch);

        uint8_t* destination = reinterpret_cast<uint8_t*>(pixels);
        
        for(int y = 0; y < GetHeight(); y++)
        {
            uint32_t* destinationRow = reinterpret_cast<uint32_t*>(destination + y * pitch);
            const uint32_t* sourceRow = mCPUData.mPixelData.data() + static_cast<size_t>(y) * GetWidth();
            std::copy_n(sourceRow, GetWidth(), destinationRow);
        }
        
        UnlockTexture();
    }

    bool Texture::TextureCPUSnapshot()
    {
        if (!mWindow.GetRenderer() || !mSDLTex)
            return false;

        SDL_Renderer* renderer = mWindow.GetRenderer();

        int width = 0;
        int height = 0;
        SDL_PixelFormatEnum outputFormat;
        
        if (SDL_QueryTexture(
                mSDLTex, reinterpret_cast<uint32_t*>(&outputFormat), nullptr, &width, &height) != 0)
        {
            return false;
        }

        void* pixels;
        int  pitch;

        // Create a temporary GPU texture that we can render into.
        SDL_Texture* readback = SDL_CreateTexture(
            renderer,
            outputFormat,
            SDL_TEXTUREACCESS_TARGET,
            width,
            height
        );

        if (!readback)
            return false;

        // Preserve the current renderer target.
        SDL_Texture* previousTarget = SDL_GetRenderTarget(renderer);

        // Preserve the source texture's rendering settings.
        SDL_BlendMode previousBlend;
        Uint8 previousR, previousG, previousB, previousA;

        SDL_GetTextureBlendMode(mSDLTex, &previousBlend);
        SDL_GetTextureColorMod(mSDLTex, &previousR, &previousG, &previousB);
        SDL_GetTextureAlphaMod(mSDLTex, &previousA);

        if (SDL_SetRenderTarget(renderer, readback) < 0)
            return false;

        // Avoid blending or modulation changing the pixel values.
        SDL_SetTextureBlendMode(mSDLTex, SDL_BLENDMODE_NONE);
        SDL_SetTextureColorMod(mSDLTex, 255, 255, 255);
        SDL_SetTextureAlphaMod(mSDLTex, 255);
        
        if (SDL_RenderCopy(renderer, mSDLTex, nullptr, nullptr) < 0)
            return false;   
        
        std::vector<uint32_t> pixelData(static_cast<size_t>(width) * height);

        // Read the rendered texture into CPU memory.
        if (SDL_RenderReadPixels(renderer, nullptr, outputFormat, pixelData.data(), width * static_cast<int>(sizeof(uint32_t))) < 0)
        {
            LogMsgf(ERROR, "failed to read render target pixels. SDL_ERROR: %s", SDL_GetError());
            return false;
        }    
        mCPUData.mPixelData = std::move(pixelData);
        mCPUData.mPixelDataFormat = outputFormat;
        mCPUData.mPixelDataWidth = width;
        mCPUData.mPixelDataHeight = height;
               
        // Restore renderer and texture state.
        SDL_SetRenderTarget(renderer, previousTarget);

        SDL_SetTextureBlendMode(mSDLTex, previousBlend);
        SDL_SetTextureColorMod(
            mSDLTex, previousR, previousG, previousB
        );
        SDL_SetTextureAlphaMod(mSDLTex, previousA);

        SDL_DestroyTexture(readback);

        return true;
    }
};
