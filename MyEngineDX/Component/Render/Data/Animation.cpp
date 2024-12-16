#include "pch.h"
#include "Animation.h"

namespace Graphics
{
	AnimationInfo::AnimationInfo(std::wstring_view _name)
		: IGraphicsResource(_name)
	{
	}
	AnimationInfo::~AnimationInfo()
	{
	}
	ChannelInfo* AnimationInfo::GetChannel(std::wstring_view _key)
	{
		auto itr = mChannels.find(_key.data());
		if (itr != mChannels.end())
		{
			return &itr->second;
		}
		return nullptr;
	}
	ChannelInfo::ChannelInfo(std::weak_ptr<AnimationInfo> _wpAnimInfo, std::wstring_view _name)
		: IGraphicsResource(_name), mOwner(_wpAnimInfo)
	{
	}
	ChannelInfo::~ChannelInfo()
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
	inline void AnimationState::SetAnimation(AnimationInfo* _pAnim)
	{
		mCurrAnimationInfo = _pAnim;
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