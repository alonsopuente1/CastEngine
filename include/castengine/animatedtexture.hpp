#pragma once

#include <vector>
#include <string>

#define FPS30 (1000 / 30)

namespace CastEngine
{

    class Texture;
    class Renderer;
    class AnimatedTexture
    {
    public:

        enum class PlaybackState : int
        {
            Stopped = 0,
            PlayingOnce,
            Looping,
            Finished
        };

    private:

        Renderer& mRender;

        std::vector<Texture*> mAnimatedTexture;

        // time for ONE FRAME to finish in milliseconds
        int mFrameTime = FPS30;

        // current frame of animation 
        int mCurrFrameIndex = 0;

        // time passed since the start of the current frame
        int mTimePassed = 0;

        PlaybackState mState = PlaybackState::Stopped;
        
    public:

        AnimatedTexture(Renderer& parentRender) : mRender(parentRender) {}
        ~AnimatedTexture() { Destroy(); }

        // move assignment and constructors
        AnimatedTexture(AnimatedTexture&& other) noexcept : mRender(other.mRender)
        {
            mAnimatedTexture = std::move(other.mAnimatedTexture);
            mFrameTime = other.mFrameTime;
            mCurrFrameIndex = other.mCurrFrameIndex;
            mTimePassed = other.mTimePassed;
            mState = other.mState;

            other.mAnimatedTexture.clear();
        }

        AnimatedTexture& operator=(AnimatedTexture&& other) noexcept
        {
            if(this != &other)
            {
                Destroy();

                mAnimatedTexture = std::move(other.mAnimatedTexture);
                mFrameTime = other.mFrameTime;
                mCurrFrameIndex = other.mCurrFrameIndex;
                mTimePassed = other.mTimePassed;
                mState = other.mState;

                other.mAnimatedTexture.clear();
            }

            return *this;
        }

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

        void PlayOnce();
        void Loop();
        void Stop();

        bool IsPlaying() const;
        bool IsStopped() const;
        bool HasFinished() const;

        Texture* GetTexture() const;
        
        inline bool IsInitialised() const { return !mAnimatedTexture.empty(); }
    };

};