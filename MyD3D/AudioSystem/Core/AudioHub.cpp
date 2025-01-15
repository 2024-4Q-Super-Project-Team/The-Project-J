#include "pch.h"
#include "AudioHub.h"
#include "AudioSystem/Core/AudioController.h"
#include "AudioSystem/Core/AudioMixer.h"

FMOD::System*			AudioHub::mCoreSystem = nullptr;
FMOD::Studio::System*	AudioHub::mSystem = nullptr;
BOOL					AudioHub::CanUse = FALSE;

BOOL AudioHub::Initialize()
{
	void* extraDriverData = NULL;
	CoInitializeEx(extraDriverData, COINIT_APARTMENTTHREADED);

	// System 객체를 사용하여 FMOD Studio API
	FMOD::Studio::System* system = NULL;
	FMOD_CHECK(FMOD::Studio::System::create(&system));

	// Core System은 하위 레벨의 FMOD 기능(예: 출력 설정, 스피커 모드 변경)을 제어.
	FMOD::System* coreSystem = NULL;
	FMOD_CHECK(system->getCoreSystem(&coreSystem));
	FMOD_CHECK(system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData)); // 1024 채널 사용

	FMOD_CHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0)); // 5.1채널 사용. 스피커가 5.1채널을 지원하지 않을 시 다운샘플링
	FMOD_CHECK(coreSystem->setDSPBufferSize(1024, 4)); // DSP버퍼 설정 (기본 값)
	FMOD_CHECK(coreSystem->set3DSettings(1.0f, 1.0f, 1.0f)); // 3D설정 (Doppler, Distance Factor, Rolloff Scale)

	CanUse = TRUE;

	mCoreSystem = coreSystem;
	mSystem = system;

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
	mCoreSystem->release();
}

void AudioHub::Update()
{
	mSystem->update();
	mCoreSystem->update();
}
