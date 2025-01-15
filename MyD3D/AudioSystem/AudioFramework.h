#pragma once
#include "AudioSystem/eAudio.h"

#include "AudioSystem/Core/AudioController.h"
#include "AudioSystem/Core/AudioHub.h"
#include "AudioSystem/Core/AudioMixer.h"

#include "AudioSystem/AudioObject/AudioChannel.h"
#include "AudioSystem/AudioObject/AudioClip.h"
#include "AudioSystem/AudioObject/AudioGroup.h"

void FMOD_CHECK(FMOD_RESULT _fr);
const char* FMOD_ErrorString(FMOD_RESULT errcode);
