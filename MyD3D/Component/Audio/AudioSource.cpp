#include "pch.h"
#include "AudioSource.h"
#include "Resource/AudioResource/AudioResource.h"

AudioSource::AudioSource(Object* _owner)
	: Component(_owner)
	, mActiveAudio(nullptr)
	, mAudioChannel(new AudioChannel)
{
}

AudioSource::~AudioSource()
{
}

void AudioSource::SetCurrentAudio(const std::wstring& _key)
{
	std::shared_ptr<AudioResource> pAudio = GetAudioFromTable(_key);
	if (pAudio)
	{
		mActiveAudio = pAudio.get();
	}
}

BOOL AudioSource::AddAudio(const std::wstring& _key, std::shared_ptr<AudioResource> _srcAudio)
{
	auto itr = mAudioTable.find(_key);
	if (FIND_FAILED(itr, mAudioTable))
	{
		mAudioTable[_key] = _srcAudio;
		// ActiveAudio가 없으면 편의상 넣어준다.
		if (mActiveAudio == nullptr)
		{
			mActiveAudio = _srcAudio.get();
		}
	}
	return FALSE;
}

std::shared_ptr<AudioResource> AudioSource::GetAudioFromTable(const std::wstring& _key)
{
	auto itr = Helper::FindMap(_key, mAudioTable);
	if (itr)
	{
		return *itr;
	}
	return nullptr;
}

bool AudioSource::IsPlaying()
{
	if (mAudioChannel)
	{
		return mAudioChannel->IsPlaying();
	}
}

bool AudioSource::IsPaused()
{
	if (mAudioChannel)
	{
		return mAudioChannel->IsPaused();
	}
}

void AudioSource::Play(std::wstring_view _key)
{
	SetCurrentAudio(_key.data());
	if (mActiveAudio)
	{
		auto pAudioClip = mActiveAudio->GetAudioClip();
		mAudioChannel->Play(pAudioClip);
	}
}

void AudioSource::Play()
{
	if (mActiveAudio)
	{
		auto pAudioClip = mActiveAudio->GetAudioClip();
		mAudioChannel->Play(pAudioClip);
	}
}

void AudioSource::Pause()
{
	if (mActiveAudio)
	{
		auto pAudioClip = mActiveAudio->GetAudioClip();
		mAudioChannel->Pause();
	}
}

void AudioSource::SetLoop(bool _isLoop)
{
	mAudioChannel->SetLoop(_isLoop);
}

void AudioSource::SetSurround(bool _isSuround)
{
	mAudioChannel->SetSurround(_isSuround);
}
