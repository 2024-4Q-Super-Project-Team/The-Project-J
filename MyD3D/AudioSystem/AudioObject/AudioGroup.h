#pragma once
#include "AudioSystem/Interface/IAudio.h"

// ����� �׷�. �ڽ��� �Ļ��� ������ �׷�� �ڽ� �׷����� ���� �� �ִ�.
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

