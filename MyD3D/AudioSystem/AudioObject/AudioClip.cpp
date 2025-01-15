#include "pch.h"
#include "AudioClip.h"
#include "AudioSystem/Core/AudioHub.h"
#include "AudioSystem/Core/AudioController.h"
#include "AudioSystem/Core/AudioMixer.h"
#include "AudioSystem/AudioObject/AudioGroup.h"

AudioClip::AudioClip(std::wstring _wPath, bool _isLoop)
	: mSound(nullptr)
	, isLoop(_isLoop)
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

void AudioClip::SetLoop(bool _isLoop)
{
	FMOD_CHECK(mSound->setMode(_isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
}

void AudioClip::SetSurround(bool _isSuround)
{
	FMOD_CHECK(mSound->setMode(_isSuround ? FMOD_3D : FMOD_2D));
}
