#include "pch.h"
#include "Animation.h"

AnimationResource::AnimationResource(std::wstring_view _name)
    : Resource(_name)
    , mTotalFrame(0.0f)
    , mFramePerSecond(0.0f)
{
}
AnimationResource::~AnimationResource()
{
}
AnimationNode* AnimationResource::GetChannel(std::wstring_view _key)
{
    auto itr = mChannels.find(_key.data());
    if (itr != mChannels.end())
    {
        return itr->second;
    }
    return nullptr;
}

BOOL AnimationResource::AddChannel(AnimationNode* _pChannel)
{
    if (GetChannel(_pChannel->GetName()) == nullptr)
    {
        mChannels[_pChannel->GetName()] = _pChannel;
        return TRUE;
    }
    return FALSE;
}

void AnimationResource::EditorRendering()
{
}

AnimationNode::AnimationNode(std::wstring_view _name, AnimationResource* _pAnimResource)
    : mName(_name), mOwner(_pAnimResource)
{
}
AnimationNode::~AnimationNode()
{
}