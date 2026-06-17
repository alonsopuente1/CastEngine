#include "gun.hpp"

#include "logger.hpp"

#include "renderer.hpp"
#include "window.hpp"
#include "texture.hpp"

namespace CastEngine
{
    
    bool Gun::Load(GunDef &def)
    {
        if(!mAnim.LoadAnimation(def.animName, def.animFilePath))
        {
            LogMsgf(ERROR, "failed to load '%s' animation", def.animName.c_str());
            return false;
        }

        if(def.fireRateMs <= 0)
        {
            LogMsgf(ERROR, "invalid firerate value. expected a value greater than 0, got '%i'", def.fireRateMs);
            return false;
        }

        mFireRateMs = def.fireRateMs;
        mAnim.SetAnimationTime(def.fireRateMs);

        if(def.damage < 0)
        {
            LogMsgf(ERROR, "invalid damage value. expected a positive value, got '%i'", def.damage);
            return false;
        }

        mDamage = def.damage;
        
        if(def.magSize < 0)
        {
            LogMsgf(ERROR, "invalid magsize value. expected positive value, got: '%i'", def.reserves);
            return false;
        }

        mAmmo = def.magSize;
        mMagSize = def.magSize;
        
        if(def.texScale < 0)
        {
            LogMsgf(ERROR, "invalid texture scale value. expected positive value got: '%f'", def.texScale);
            return false;
        }
        mTexScale = def.texScale;
        
        if(def.reserves < 0)
        {
            LogMsgf(ERROR, "invalid reserves value. expected a positive value, got: '%i'", def.reserves);
            return false;
        }
        mReserves = def.reserves;

        return true;
    }

    void Gun::Update(float dtMs)
    {
        if(mCooldownMs < mFireRateMs)
            mCooldownMs += dtMs;

        mAnim.Update(dtMs);
    }

    void Gun::Draw(Renderer &render)
    {
        Texture* tex = mAnim.GetTexture();
        if(!tex || !tex->IsInitialised()) return;
        
        Window& window = render.GetWindow();
    
        int windowW = window.GetWidth();
        int windowH = window.GetHeight();

        float ratio = static_cast<float>(windowW) / (2.f * tex->GetWidth());

        int dstW = static_cast<int>(tex->GetWidth() * ratio * mTexScale);
        int dstH = static_cast<int>(tex->GetHeight() * ratio * mTexScale);

        int dstX = static_cast<int>(windowW * 0.5f - dstW * 0.5f);
        int dstY = static_cast<int>(windowH - dstH);

        SDL_Rect src { 0, 0,
            static_cast<int>(tex->GetWidth()),
            static_cast<int>(tex->GetHeight()) };
        SDL_Rect dst { dstX, dstY, dstW, dstH };

        if(!render.RenderTexture(*tex, src, dst))
            LogMsg(ERROR, "failed to render gun");
    }
 
    bool Gun::TryShoot()
    {
        if(mAmmo <= 0 || !IsReady())
            return false;
        mCooldownMs = 0;
        mAnim.PlayOnce();
        mAmmo--;

        return true;
    }
    
    void Gun::Reload()
    {
        if(mReserves <= 0)
            return;

        int bulletsNeeded = mMagSize - mAmmo;

        if(bulletsNeeded == 0)
            return;

        if(bulletsNeeded > mReserves)
            bulletsNeeded = mReserves;

        mReserves -= bulletsNeeded;
        mAmmo += bulletsNeeded;
    }
}
