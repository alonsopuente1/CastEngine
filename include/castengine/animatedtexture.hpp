#pragma once

#include <vector>
#include <string>

namespace CastEngine
{

    class Texture;
    class Renderer;
    class AnimatedTexture
    {

    private:

        Renderer& mRender;

        std::vector<Texture*> mAnimatedTexture;

        // time for ONE FRAME to finish in milliseconds
        int mFrameTime = 0;

        // current frame of animation 
        int mCurrFrameIndex = 0;

        // time passed since the start of the current frame
        int mTimePassed = 0;

        // if true, animation loops indefinitely until stopped
        bool mLooping = false;
        
    public:

        AnimatedTexture(Renderer& parentRender) : mRender(parentRender) {}
        ~AnimatedTexture() { Destroy(); }

        bool LoadAnimation(const std::string& animName, const std::string& filePath);

        /// @brief deallocates the textures from the parent renderer's texturebank
        void Destroy();

        /// @brief sets the time it takes to render ONE frame
        /// @param newTimeMs time in milliseconds
        void SetFrameTime(int newTimeMs);

        /// @brief sets the time it takes to complete this animation
        /// @param newTimeMs time in milliseconds
        void SetAnimationTime(int newTimeMs);

        void Update(float dtMs);

        Texture* GetTexture() const;
        
        inline void PlayOnce() { mCurrFrameIndex = 1; }
        inline void Loop() { mLooping = true; }

        inline bool IsPlaying() const { return mCurrFrameIndex > 0 || mLooping; }
        inline bool IsInitialised() const { return !mAnimatedTexture.empty(); }
    };

};