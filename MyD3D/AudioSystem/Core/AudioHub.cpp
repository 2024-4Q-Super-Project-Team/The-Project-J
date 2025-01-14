#include "pch.h"
#include "AudioHub.h"
#include "AudioSystem/Core/AudioController.h"
#include "AudioSystem/Core/AudioMixer.h"

FMOD::System*	AudioHub::mSystem = nullptr;
BOOL			AudioHub::CanUse = FALSE;

BOOL AudioHub::Initialize()
{
	if (FMOD_OK != FMOD::System_Create(&mSystem))
	{
		return FALSE;
	}

	CanUse = TRUE;

	if (FALSE == AudioController::Initialize())
	{
		return FALSE;
	}

	if (FALSE == AudioMixer::Initialize())
	{
		return FALSE;
	}

	return CanUse;
}

void AudioHub::Finalization()
{
	mSystem->release();
}

void AudioHub::Update()
{
	mSystem->update();
}
