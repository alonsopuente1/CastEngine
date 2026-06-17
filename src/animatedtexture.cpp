#include "animatedtexture.hpp"

#include "texture.hpp"
#include "util.hpp"
#include "renderer.hpp"
#include "logger.hpp"

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
        if(!mLooping && mCurrFrameIndex == 0)
            return;

        mTimePassed += dtMs;

        while(mTimePassed > mFrameTime)
        {
            if(++mCurrFrameIndex >= static_cast<int>(mAnimatedTexture.size()))
                mCurrFrameIndex = 0;
            mTimePassed -= mFrameTime;
        }
    }

    Texture *AnimatedTexture::GetTexture() const
    {
        if(mAnimatedTexture.empty())
            return nullptr;
    
        return mAnimatedTexture[mCurrFrameIndex];
    }
};