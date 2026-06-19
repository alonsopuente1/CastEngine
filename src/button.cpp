#include "castengine/button.hpp"

#include "castengine/window.hpp"
#include "castengine/renderer.hpp"
#include "castengine/fonts.hpp"
#include "castengine/logger.hpp"

CastEngine::Button::Button(Window &pWnd, Renderer &pRend) : mParentWindow(pWnd),
    mParentRenderer(pRend), mText(nullptr)
{
}

CastEngine::Button::Button(Button &&other) noexcept : 
    mParentWindow(other.mParentWindow),
    mParentRenderer(other.mParentRenderer),
    mPos(other.mPos),
    mBackgroundColour(other.mBackgroundColour),
    mText(other.mText),
    mOnClick(other.mOnClick)
{
    other.mText = nullptr;
    other.mPos = { 0, 0, 0, 0};
    other.mOnClick = std::function<void()>();
    other.mBackgroundColour = { 0, 0, 0, 0 };
}

CastEngine::Button &CastEngine::Button::operator=(Button &&other) noexcept
{
    if(mParentWindow != other.mParentWindow || mParentRenderer != other.mParentRenderer)
    {
        LogMsg(ERROR, "trying to move button created on a different renderer");
        exit(-1);
    }

    mOnClick = other.mOnClick;
    mBackgroundColour = other.mBackgroundColour;
    mPos = other.mPos;
    mText = other.mText;

    other.mText = nullptr;
    other.mOnClick = std::function<void()>();
    other.mPos = { 0, 0, 0, 0 };
    other.mBackgroundColour = { 0, 0, 0, 0 };

    return *this;
}

CastEngine::Button::~Button()
{
    Destroy();
}

void CastEngine::Button::SetText(const std::string &newText)
{
    if(mText)
    {
        mParentRenderer.texBank.RemoveByTex(*mText);
        mText = nullptr;
    }

    if(newText.empty())
    {
        return;
    }

    if(CastEngine::fonts.empty())
    {
        LogMsg(ERROR, "no fonts loaded. cannot set button text\n");
        return;
    }

    mText = CastEngine::CreateText(mParentRenderer, {255, 255, 255, 255}, CastEngine::fonts[0], newText);
    if(!mText)
    {
        LogMsgf(ERROR, "failed to create text for button with text '%s'\n", newText.c_str());
    }
}

void CastEngine::Button::SetPosition(SDL_Rect newPos)
{
    mPos = newPos;
}

void CastEngine::Button::SetBackgroundColour(SDL_Color newColour)
{
    mBackgroundColour = newColour;
}

void CastEngine::Button::HandleEvent(const SDL_Event &e)
{
    if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePos = {mouseX, mouseY};
        if(SDL_PointInRect(&mousePos, &mPos) && mOnClick)
            mOnClick();
    }
}

void CastEngine::Button::Draw()
{

    SDL_SetRenderDrawBlendMode(mParentRenderer.GetWindow().GetRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(mParentRenderer.GetWindow().GetRenderer(), 
        mBackgroundColour.r, 
        mBackgroundColour.g, 
        mBackgroundColour.b, 
        mBackgroundColour.a
    );


    SDL_RenderFillRect(mParentRenderer.GetWindow().GetRenderer(), &mPos);
    if(mText)
    {
        mParentRenderer.RenderTexture(*mText, {0, 0, static_cast<int>(mText->GetWidth()), static_cast<int>(mText->GetHeight())}, mPos);
    }
    SDL_SetRenderDrawBlendMode(mParentRenderer.GetWindow().GetRenderer(), SDL_BLENDMODE_NONE);    
}

void CastEngine::Button::Destroy()
{
    if(mText)
    {
        mParentRenderer.texBank.RemoveByTex(*mText);
        mText = nullptr;
    }
}
