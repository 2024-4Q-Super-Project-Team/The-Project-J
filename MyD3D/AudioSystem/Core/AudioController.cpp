#include "pch.h"
#include "AudioController.h"
#include "AudioSystem/Core/AudioHub.h"
#include "AudioSystem/AudioObject/AudioChannel.h"
#include "AudioSystem/AudioObject/AudioGroup.h"

INT AudioController::mNumChannel = 0;
std::vector<AudioChannel*>			AudioController::mAllChannels;
std::unordered_set<AudioChannel*> 	AudioController::mActiveChannels;
std::queue<AudioChannel*>			AudioController::mAvailableChannels;

AudioController::AudioController()
{
}

AudioController::~AudioController()
{
}

BOOL AudioController::Initialize()
{
	auto pSystem = AudioHub::GetCoreSystem();
	// 채널 최대 개수를 받아와서 System초기화
	if (FMOD_OK != pSystem->getSoftwareChannels(&mNumChannel))
	{
		return FALSE;
	}
	
	//mAllChannels.reserve(mNumChannel);
	//mActiveChannels.reserve(mNumChannel);
	//for (int i = 0; i < mNumChannel; ++i)
	//{
	//	AudioChannel* pChannel = new AudioChannel;
	//	mAvailableChannels.push(pChannel);
	//	mAllChannels.push_back(pChannel);
	//}
	return TRUE;
}

void AudioController::Finalization()
{
	SAFE_DELETE_VECTOR(mAllChannels);
	mActiveChannels.clear();
	while (!mAvailableChannels.empty())
	{
		mAvailableChannels.pop();
	}
}

HRESULT AudioController::QueryChannel(OUT AudioChannel** _ppChannel)
{
	if (mAvailableChannels.empty())
	{
		return E_FAIL;
	}
	(*_ppChannel) = mAvailableChannels.front();
	mActiveChannels.insert(mAvailableChannels.front());
	mAvailableChannels.pop();
	return S_OK;
}
