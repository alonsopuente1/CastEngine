#include "gun.hpp"

#include "logger.hpp"

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
    }
    bool Gun::TryShoot()
    {
        if(mAmmo <= 0 || !IsReady())
            return false;

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
