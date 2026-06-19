#pragma once

#include "castengine/animatedtexture.hpp"


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
    
    std::string name;

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

    CastEngine::AnimatedTexture mAnim;
    
    std::string mName;

public:

    Gun(CastEngine::Renderer& rend) : mAnim(rend) {}
    ~Gun() { mAnim.Destroy(); }

    bool Load(GunDef& def);
    void Update(float dtMs);
    void Draw(CastEngine::Renderer& render);
        
    bool TryShoot();
    void Reload();

    inline bool IsReady() { return mCooldownMs >= mFireRateMs; }

    inline int GetDamage() { return mDamage; }

    inline int GetAmmo() const { return mAmmo; }
    inline int GetMagSize() const { return mMagSize; }
    inline int GetReserves() const { return mReserves; }
    inline const std::string& GetName() const { return mName; }
};
