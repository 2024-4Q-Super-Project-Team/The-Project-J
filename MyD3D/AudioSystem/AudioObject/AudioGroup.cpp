#include "pch.h"
#include "AudioGroup.h"
#include "AudioSystem/Core/AudioHub.h"
#include "AudioSystem/Core/AudioController.h"
#include "AudioSystem/Core/AudioMixer.h"

AudioGroup::AudioGroup(std::wstring_view _groupName)
	: mGroupName(_groupName)
	, mGroup(nullptr)
	, mSubGroup({})
	, mGroupVolume(1.0f)
{
}

AudioGroup::~AudioGroup()
{
	if (mGroup)
	{
		mGroup->release();
		mGroup = nullptr;
	}
}

HRESULT AudioGroup::Create()
{
	std::string name;
	name.assign(mGroupName.begin(), mGroupName.end());
	if (FMOD_OK != AudioHub::GetCoreSystem()->createChannelGroup(name.data(), &mGroup))
	{
		return E_FAIL;
	}
	return S_OK;
}

void AudioGroup::Release()
{
	if (mGroup)
	{
		mGroup->release();
		mGroup = nullptr;
	}
	delete this;
}

AudioGroup* AudioGroup::AddChildGroup(const std::wstring& _wGroupName)
{
	AudioGroup* pGroup = new AudioGroup(_wGroupName);
	if (FMOD_OK != mGroup->addGroup(pGroup->mGroup))
	{
		return nullptr;
	}
	mSubGroup[_wGroupName.data()] = pGroup;
	return pGroup;
}

AudioGroup* AudioGroup::GetChildGroup(const std::wstring& _wGroupName)
{
	auto itr = Helper::FindMap(_wGroupName, mSubGroup);
	if (itr)
	{
		return (*itr);
	}
	else
	{
		return nullptr;
	}
}
