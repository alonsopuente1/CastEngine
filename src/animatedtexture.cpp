#include "castengine/animatedtexture.hpp"

#include "castengine/texture.hpp"
#include "castengine/renderer.hpp"
#include "castengine/util.hpp"
#include "castengine/logger.hpp"

#include <algorithm>

namespace CastEngine
{
    bool AnimatedTexture::LoadAnimation(const std::string &animName, const std::string &filePath)
    {
        std::vector<std::string> frameFilePaths;
        if(!GetAllFilesInDir(filePath, frameFilePaths))
            return false;

        std::sort(frameFilePaths.begin(), frameFilePaths.end());

        if(frameFilePaths.empty())
            return false;

        for(const auto& framePath : frameFilePaths)
        {
            mAnimatedTexture.push_back(mRender.texBank.PushTexture(Texture(mRender.GetWindow(), framePath)));
            
            if(mAnimatedTexture.back() == nullptr)
            {
                LogMsgf(ERROR, "failed to load texture at file path '%s'", framePath.c_str());
                Destroy();
                return false;
            }
        }

        return true;
    }

    void AnimatedTexture::Destroy()
    {
        for(auto& tex : mAnimatedTexture)
            mRender.texBank.RemoveByTex(*tex);

        mAnimatedTexture.clear();
    }

    void AnimatedTexture::SetFrameTime(int newTimeMs)
    {
        if(!IsInitialised())
        {
            LogMsg(WARN, "attempting to set frame time when no animation is loaded");
            return;
        }

        mFrameTime = newTimeMs;
    }
    
    void AnimatedTexture::SetAnimationTime(int newTimeMs)
    {
        if(!IsInitialised())
        {
            LogMsg(WARN, "attempting to set animation time when no animation is loaded");
            return;
        }

        SetFrameTime(newTimeMs / static_cast<int>(mAnimatedTexture.size()));
    }

    void AnimatedTexture::Update(float dtMs)
    {
        if(IsStopped() || !IsInitialised() || mFrameTime <= 0)
            return;

        mTimePassed += dtMs;

        while(mTimePassed >= mFrameTime && IsPlaying())
        {
            mTimePassed -= mFrameTime;
            
            if(mCurrFrameIndex + 1 < static_cast<int>(mAnimatedTexture.size()))
                ++mCurrFrameIndex;
            else if(mState == PlaybackState::Looping)
                mCurrFrameIndex = 0;
            else
            {
                mState = PlaybackState::Finished;
                mTimePassed = 0;
            }
        }
    }

    void AnimatedTexture::PlayOnce()
    {
        if(!IsInitialised())
            return;

        mState = PlaybackState::PlayingOnce;
        mCurrFrameIndex = 0;
        mTimePassed = 0;        
    }
    void AnimatedTexture::Loop()
    {
        if(!IsInitialised())
            return;

        mState = PlaybackState::Looping;
        mCurrFrameIndex = 0;
        mTimePassed = 0;
    }
    
    bool AnimatedTexture::IsPlaying() const
    {
        return mState == PlaybackState::Looping || mState == PlaybackState::PlayingOnce;
    }
    
    bool AnimatedTexture::IsStopped() const
    {
        return !IsPlaying();
    }

    void AnimatedTexture::Stop()
    {
        mCurrFrameIndex = 0;
        mTimePassed = 0;
        mState = PlaybackState::Stopped;
    }
    
    bool AnimatedTexture::HasFinished() const
    {
        return mState == PlaybackState::Finished;
    }

    Texture *AnimatedTexture::GetTexture() const
    {
        if(!IsInitialised())
            return nullptr;
    
        return mAnimatedTexture[mCurrFrameIndex];
    }
};