#pragma once
#include "AudioSystem/Interface/IAudio.h"

// 오디오 그룹. 자식을 파생해 마스터 그룹과 자식 그룹으로 나눌 수 있다.
class AudioGroup : public IAudioResource
{
public:
	explicit AudioGroup(std::wstring_view _groupName);
	virtual ~AudioGroup();
public:
	virtual HRESULT Create() override;
	virtual void	Release() override;
public:
	AudioGroup* AddChildGroup(const std::wstring& _wGroupName);
	AudioGroup* GetChildGroup(const std::wstring& _wGroupName);
public:
	inline FLOAT GetGroupVolume() { return mGroupVolume; }
	inline void  SetGroupVolume(FLOAT _val) { mGroupVolume = Clamp(_val, 0.0f, 1.0f); }
public:
	std::wstring mGroupName;
	FMOD::ChannelGroup* mGroup;
	std::unordered_map<std::wstring, AudioGroup*> mSubGroup;
	FLOAT mGroupVolume;
};

