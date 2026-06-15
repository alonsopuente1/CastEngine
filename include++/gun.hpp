#pragma once

#include "animatedtexture.hpp"

namespace CastEngine
{

    struct GunDef
    {

        std::string animFilePath; // file path to folder containing
                                    // gun shooting frames
        std::string animName;

        int fireRateMs;
        float texScale;     // value to scale gun textures by
        int damage;

        int magSize;
        int reserves;

    };

    class Gun
    {
    
    private:

        int mAmmo;       // amount of bullets currently in magazine
        int mReserves;   // amount of bullets left over
        int mMagSize;    // max size of magazine

        int mCooldownMs; // time since gun last been shot
        int mFireRateMs; // time before gun can shoot again

        int mDamage;
        float mTexScale;

        AnimatedTexture mAnim;
    public:

        Gun(Renderer& rend) : mAnim(rend) {}
        ~Gun() { mAnim.Destroy(); }

        bool Load(GunDef& def);
        void Update(float dtMs);

        bool TryShoot();
        void Reload();

        inline bool IsReady() { return mCooldownMs >= mFireRateMs; }

        inline int GetDamage() { return mDamage; }


    };

    

};