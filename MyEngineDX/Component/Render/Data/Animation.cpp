#include "pch.h"
#include "Animation.h"

namespace Graphics
{
	AnimationResource::AnimationResource(std::wstring_view _name)
		: IGraphicsResource(_name)
	{
	}
	AnimationResource::~AnimationResource()
	{
	}
	ChannelResource* AnimationResource::GetChannel(std::wstring_view _key)
	{
		auto itr = mChannels.find(_key.data());
		if (itr != mChannels.end())
		{
			return &itr->second;
		}
		return nullptr;
	}
	ChannelResource::ChannelResource(std::weak_ptr<AnimationResource> _wpAnimResource, std::wstring_view _name)
		: IGraphicsResource(_name), mOwner(_wpAnimResource)
	{
	}
	ChannelResource::~ChannelResource()
	{
	}
	AnimationState::AnimationState()
	{
	}
	AnimationState::~AnimationState()
	{
	}
	void AnimationState::UpdateState()
	{
		mTimer += Time::GetScaledDeltaTime();
	}
	inline void AnimationState::SetAnimation(AnimationResource* _pAnim)
	{
		mCurrAnimationResource = _pAnim;
	}
	inline void AnimationState::SetTimer(float _time)
	{
		mTimer = _time;
	}
	inline float AnimationState::GetNormalizedTime()
	{
		return 0.0f;
	}
	inline float AnimationState::GetTimer()
	{
		return mTimer;
	}
}