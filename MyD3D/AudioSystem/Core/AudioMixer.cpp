#include "pch.h"
#include "AudioMixer.h"
#include "AudioSystem/AudioObject/AudioChannel.h"
#include "AudioSystem/AudioObject/AudioGroup.h"

AudioGroup* AudioMixer::mMasterGroup;

BOOL AudioMixer::Initialize()
{
	mMasterGroup = new AudioGroup(L"Mater_Group");
	mMasterGroup->Create();
	return TRUE;
}

void AudioMixer::Finalization()
{
	mMasterGroup->Release();
}

AudioGroup* AudioMixer::GetMasterGroup()
{
	return mMasterGroup;
}

AudioGroup* AudioMixer::AddGroup(const std::wstring& _wGroupName)
{
	mMasterGroup->AddChildGroup(_wGroupName);
	return nullptr;
}

AudioGroup* AudioMixer::GetGroup(const std::wstring& _wGroupName)
{
	return mMasterGroup->GetChildGroup(_wGroupName);
}
