#include "pch.h"
#include "AudioChannel.h"
#include "AudioSystem/Core/AudioHub.h"
#include "AudioSystem//AudioObject/AudioClip.h"
#include "AudioSystem//AudioObject/AudioGroup.h"

AudioChannel::AudioChannel()
	: mChannel(nullptr)
	, mFrame(0)
	, isPaused(false)
	, mVolume(1.0f)
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
	FMOD::System* pSystem = AudioHub::GetCoreSystem();
	if (!pSystem) return E_FAIL;
	if (_pAudio == nullptr) return E_FAIL;
	if (_pAudio->GetGroup() == nullptr) return E_FAIL;
	if (_pAudio->GetGroup()->mGroup == nullptr) return E_FAIL;
	if (_pAudio->GetSound() == nullptr) return E_FAIL;

	FMOD_CHECK(pSystem->playSound(
		_pAudio->GetSound(),
		_pAudio->GetGroup()->mGroup,
		false,
		&mChannel)
	);
	mChannel->setVolume(mVolume);
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
		UINT position = 0;
		mChannel->getPosition(&position, FMOD_TIMEUNIT_MS);
		mChannel->setPaused(true);
		mFrame = position;
		isPaused = true;
		return S_OK;
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

void AudioChannel::SetLoop(bool _isLoop)
{
	FMOD_CHECK(mChannel->setMode(_isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
}

void AudioChannel::SetSurround(bool _isSuround)
{
	FMOD_CHECK(mChannel->setMode(_isSuround ? FMOD_3D : FMOD_2D));
}

void AudioChannel::SetPosition(Vector3 _position, Vector3 _velocity)
{
	FMOD_VECTOR pos = { _position.x, _position.y, _position.z };
	FMOD_VECTOR vel = { _velocity.x, _velocity.y, _velocity.z };
	mChannel->set3DAttributes(&pos, &vel);
}

void AudioChannel::SetVolume(FLOAT _val)
{
	if (mChannel)
	{
		mVolume = _val;
		if (IsPlaying())
		{
			mChannel->setVolume(mVolume);
		}
	}

	// FMOD 시스템 업데이트
	FMOD::System* pSystem = AudioHub::GetCoreSystem();
	if (pSystem)
	{
		pSystem->update();
	}
}
