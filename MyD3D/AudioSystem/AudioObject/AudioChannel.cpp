#include "pch.h"
#include "AudioChannel.h"
#include "AudioSystem/Core/AudioHub.h"
#include "AudioSystem//AudioObject/AudioClip.h"
#include "AudioSystem//AudioObject/AudioGroup.h"

AudioChannel::AudioChannel()
	: mChannel(nullptr)
	, mFrame(0)
	, isPaused(false)
{
}

AudioChannel::~AudioChannel()
{
}

HRESULT AudioChannel::Create()
{
	return E_NOTIMPL;
}

void AudioChannel::Release()
{
}

bool AudioChannel::IsPlaying()
{
	if (mChannel)
	{
		bool isPlay = false;
		mChannel->isPlaying(&isPlay);
		return isPlay;
	}
	return false;
}

bool AudioChannel::IsPaused()
{
	return isPaused;
}

HRESULT AudioChannel::Play(AudioClip* _pAudio)
{
	FMOD::System* pSystem = AudioHub::GetSystem();
	if (!pSystem) return E_FAIL;
	if (_pAudio == nullptr) return E_FAIL;
	if (_pAudio->GetGroup() == nullptr) return E_FAIL;
	if (_pAudio->GetGroup()->mGroup == nullptr) return E_FAIL;
	if (_pAudio->GetSound() == nullptr) return E_FAIL;

	FMOD_RESULT fr = pSystem->playSound(
		_pAudio->GetSound(),
		_pAudio->GetGroup()->mGroup,
		false,
		&mChannel);

	if (FMOD_OK != fr)
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT AudioChannel::Reset()
{
	if (mChannel)
	{
		if (IsPlaying())
		{
			// stop()이 자동으로 가비지컬렉션을 해주기 때문에 release호출이 불필요
			mChannel->stop();
			mChannel = nullptr;
			mFrame = 0;
			isPaused = false;
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT AudioChannel::Pause()
{
	if (mChannel)
	{
		if (!isPaused)
		{
			UINT position = 0;
			mChannel->getPosition(&position, FMOD_TIMEUNIT_MS);
			mChannel->setPaused(true);
			mFrame = position;
			isPaused = true;
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT AudioChannel::Resume()
{
	if (mChannel) 
	{
		if (isPaused)
		{
			// 저장된 위치에서 재생 시작
			mChannel->setPosition(mFrame, FMOD_TIMEUNIT_MS);
			mChannel->setPaused(false);
			isPaused = false;
			return S_OK;
		}
	}
	return E_FAIL;
}
