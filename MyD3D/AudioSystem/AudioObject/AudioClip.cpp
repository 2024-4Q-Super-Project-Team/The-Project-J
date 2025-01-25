#include "pch.h"
#include "AudioClip.h"
#include "AudioSystem/Core/AudioHub.h"
#include "AudioSystem/Core/AudioController.h"
#include "AudioSystem/Core/AudioMixer.h"
#include "AudioSystem/AudioObject/AudioGroup.h"

AudioClip::AudioClip(std::wstring _wPath, bool _isLoop)
	: mSound(nullptr)
	, mPath(_wPath)
	, mGroup(nullptr)
{
	mGroup = AudioMixer::GetMasterGroup();
}

AudioClip::~AudioClip()
{
	if (mSound)
	{
		mSound->release();
		mSound = nullptr;
	}
}

HRESULT AudioClip::Create()
{
	std::string path;
	path.assign(mPath.begin(), mPath.end());
	FMOD_CHECK(AudioHub::GetCoreSystem()->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &mSound));
	return S_OK;
}

void AudioClip::Release()
{
	if (mSound)
	{
		mSound->release();
		mSound = nullptr;
	}
	delete this;
}

void AudioClip::SetGroup(AudioGroup* _pGroup)
{
	if (_pGroup)
	{
		mGroup = _pGroup;
	}
}

void AudioClip::SetLoop(BOOL _isLoop)
{
	FMOD_CHECK(mSound->setMode(_isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
}

void AudioClip::SetSoundMode(eAudioListenMode _mode)
{
	switch (_mode)
	{
	case eAudioListenMode::DEFAULT:
		FMOD_CHECK(mSound->setMode(FMOD_DEFAULT));
		break;
	case eAudioListenMode::SOUND_2D:
		FMOD_CHECK(mSound->setMode(FMOD_2D));
		break;
	case eAudioListenMode::SOUND_3D:
		FMOD_CHECK(mSound->setMode(FMOD_3D));
		break;
	default:
		FMOD_CHECK(mSound->setMode(FMOD_DEFAULT));
		break;
	}
}