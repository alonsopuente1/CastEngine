#include "castengine/texture.hpp"

#include <SDL2/SDL_image.h>

#include <cstring>

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

    Texture::Texture(Texture &&other) noexcept : mCPUTex(other.mCPUTex), mSDLTex(other.mSDLTex), mName(other.mName), mWindow(other.mWindow)
    {
        other.mCPUTex = NULL;
        other.mSDLTex = NULL;
        other.mName = "";
    }

    Texture &Texture::operator=(Texture &&tex) noexcept
    {
        if(this == &tex)
            return *this;

        Destroy();

        mSDLTex = tex.mSDLTex;
        mCPUTex = tex.mCPUTex;

        tex.mSDLTex = NULL;
        tex.mCPUTex = NULL;

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
        if(mCPUTex)
            SDL_FreeSurface(mCPUTex);
        mSDLTex = nullptr;
        mName = "";
        mWidth = 0;
        mHeight = 0;
    }
    
    bool Texture::IsInitialised() const
    {
        return mSDLTex != NULL || mCPUTex != NULL;
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

    SDL_Surface *Texture::GetSurface() const
    {
        if(mCPUTexLocked)
        {
            LogMsg(WARN, "cannot get surface, surface is locked");
            return nullptr;
        }
        return mCPUTex;
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
        mCPUTex = newSurface;

        if(SDL_QueryTexture(mSDLTex, NULL, NULL, &mWidth, &mHeight) < 0)
        {
            LogMsgf(ERROR, "failed to retrieve width and height of texture (%s). SDL_ERROR: %s", filePath.c_str(), SDL_GetError());
            Destroy();
            return false;
        }

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
        mCPUTex = newSurface; 
        SetTextureName(pName);

        mWidth = pWidth;
        mHeight = pHeight;

        return true;
    }

    bool Texture::LockSurface()
    {
        if(mCPUTexLocked)
            return true;
        if(!mCPUTex)
        {
            LogMsg(ERROR, "attempting to lock an uninitialised surface");
            return false;
        }
        if(SDL_LockSurface(mCPUTex) < 0)
        {
            LogMsgf(ERROR, "failed to lock surface. SDL_ERROR: %s", SDL_GetError());
            return false;
        }

        mCPUTexLocked = true;
        return true;
    }

    void Texture::UnlockSurface()
    {
        SDL_UnlockSurface(mCPUTex);
        mCPUTexLocked = false;
    }

    bool Texture::LockTexture(const SDL_Rect *rect, void **pixels, int *pitch)
    {
        if(mSDLTexLocked)
            return true;
        if(SDL_LockTexture(mSDLTex, rect, pixels, pitch) < 0)
        {
            LogMsgf(ERROR, "failed to lock texture. SDL_ERROR: %s", SDL_GetError());
            mSDLTexLocked = true;
            return false;
        }
        return true;
    }

    void Texture::UnlockTexture()
    {
        SDL_UnlockTexture(mSDLTex);
        mSDLTexLocked = false;
    }

    bool Texture::SyncTextureToSurface()
    {
        if(!mSDLTex || !mCPUTex)
        {
            LogMsg(ERROR, "attempting to sync an uninitialised texture");
            return false;
        }
        if(mSDLTexLocked)
           UnlockTexture();
        if(!mCPUTexLocked)
        {
            if(!LockSurface())
                return false;
        }

        if(SDL_UpdateTexture(mSDLTex, NULL, mCPUTex->pixels, mCPUTex->pitch) < 0)
        {
            LogMsgf(ERROR, "failed to sync surface to texture. SDL_ERROR: %s", SDL_GetError());
            UnlockSurface();
            return false;
        }

        UnlockSurface();

        return true;
    }

    Texture::Colour Texture::GetColour(int x, int y)
    {
        Colour output = {};

        if(!mCPUTex)
        {
            LogMsg(ERROR, "attempting to get a colour from an uninitialised surface");
            return output;
        }
        if(!mCPUTexLocked)
        {
            LogMsg(WARN, "cannot get colour from an unlocked surface");
            return output;
        }
        if(x < 0 || x >= mCPUTex->w || y < 0 || y >= mCPUTex->h)
        {
            LogMsgf(WARN, "pixel coordinates (%d, %d) are outside texture bounds", x, y);
            return output;
        }

        const int bytesPerPixel = mCPUTex->format->BytesPerPixel;
        const Uint8* pixelBytes = static_cast<const Uint8*>(mCPUTex->pixels)
            + y * mCPUTex->pitch + x * bytesPerPixel;
        Uint32 pixel = 0;

        switch(bytesPerPixel)
        {
            case 1:
                pixel = *pixelBytes;
                break;
            case 2:
                std::memcpy(&pixel, pixelBytes, sizeof(Uint16));
                break;
            case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                pixel = (static_cast<Uint32>(pixelBytes[0]) << 16)
                    | (static_cast<Uint32>(pixelBytes[1]) << 8)
                    | static_cast<Uint32>(pixelBytes[2]);
#else
                pixel = static_cast<Uint32>(pixelBytes[0])
                    | (static_cast<Uint32>(pixelBytes[1]) << 8)
                    | (static_cast<Uint32>(pixelBytes[2]) << 16);
#endif
                break;
            case 4:
                std::memcpy(&pixel, pixelBytes, sizeof(pixel));
                break;
            default:
                LogMsgf(ERROR, "unsupported pixel size (%d bytes) in texture", bytesPerPixel);
                return output;
        }

        SDL_GetRGBA(pixel, mCPUTex->format, &output.r, &output.g, &output.b, &output.a);

        return output;
    }

    bool Texture::SetColour(Colour colour, int x, int y)
    {
        if(!mCPUTex)
        {
            LogMsg(ERROR, "attempting to set a colour on an uninitialised surface");
            return false;
        }
        if(!mCPUTexLocked)
        {
            LogMsg(WARN, "attempting to set colour when surface isnt locked!");
            return false;
        }
        if(x < 0 || x >= mCPUTex->w || y < 0 || y >= mCPUTex->h)
        {
            LogMsgf(WARN, "pixel coordinates (%d, %d) are outside texture bounds", x, y);
            return false;
        }

        const Uint32 pixel = SDL_MapRGBA(mCPUTex->format, colour.r, colour.g, colour.b, colour.a);
        const int bytesPerPixel = mCPUTex->format->BytesPerPixel;
        Uint8* pixelBytes = static_cast<Uint8*>(mCPUTex->pixels)
            + static_cast<size_t>(y) * mCPUTex->pitch
            + static_cast<size_t>(x) * bytesPerPixel;
        
        switch(bytesPerPixel)
        {
            case 1:
                *pixelBytes = static_cast<Uint8>(pixel);
                break;
            case 2:
            {
                const Uint16 pixel16 = static_cast<Uint16>(pixel);
                std::memcpy(pixelBytes, &pixel16, sizeof(pixel16));
                break;
            }
            case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                pixelBytes[0] = static_cast<Uint8>((pixel >> 16) & 0xff);
                pixelBytes[1] = static_cast<Uint8>((pixel >> 8) & 0xff);
                pixelBytes[2] = static_cast<Uint8>(pixel & 0xff);
#else
                pixelBytes[0] = static_cast<Uint8>(pixel & 0xff);
                pixelBytes[1] = static_cast<Uint8>((pixel >> 8) & 0xff);
                pixelBytes[2] = static_cast<Uint8>((pixel >> 16) & 0xff);
#endif
                break;
            case 4:
                std::memcpy(pixelBytes, &pixel, sizeof(pixel));
                break;
            default:
                LogMsgf(ERROR, "unsupported pixel size (%d bytes) in texture", bytesPerPixel);
                return false;
        }

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

};
