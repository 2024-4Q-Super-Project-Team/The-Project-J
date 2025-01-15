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

	// System ��ü�� ����Ͽ� FMOD Studio API
	FMOD::Studio::System* system = NULL;
	FMOD_CHECK(FMOD::Studio::System::create(&system));

	// Core System�� ���� ������ FMOD ���(��: ��� ����, ����Ŀ ��� ����)�� ����.
	FMOD::System* coreSystem = NULL;
	FMOD_CHECK(system->getCoreSystem(&coreSystem));
	FMOD_CHECK(system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData)); // 1024 ä�� ���

	FMOD_CHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0)); // 5.1ä�� ���. ����Ŀ�� 5.1ä���� �������� ���� �� �ٿ���ø�
	FMOD_CHECK(coreSystem->setDSPBufferSize(1024, 4)); // DSP���� ���� (�⺻ ��)
	FMOD_CHECK(coreSystem->set3DSettings(1.0f, 1.0f, 1.0f)); // 3D���� (Doppler, Distance Factor, Rolloff Scale)

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
