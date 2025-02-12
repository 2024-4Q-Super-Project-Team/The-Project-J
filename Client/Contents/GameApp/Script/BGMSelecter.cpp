#include "pch.h"
#include "BGMSelecter.h"

AudioSource* BGMSelecter::mCurrAudio = nullptr;
AudioSource* BGMSelecter::AudioSourceArray[(UINT)eBGMType::NONE];

#define BGM_STAGE_OPENING L"BGM_Opening.mp3"
#define BGM_STAGE_TITLE L"BGM_Title.mp3"
#define BGM_STAGE_01 L"BGM_Stage(1).mp3"
#define BGM_STAGE_02 L"BGM_Stage(2).mp3"
#define BGM_STAGE_BOSS L"BGM_Boss.mp3"
#define BGM_STAGE_ENDING L"BGM_Ending.mp3"

void BGMSelecter::Start()
{
	InitSFX();
}

void BGMSelecter::InitSFX()
{
	{
		AudioSource* pAudioSource = gameObject->AddComponent<AudioSource>();
		pAudioSource->SetVolume(0.3f);
		LoadBGM(pAudioSource, BGM_STAGE_OPENING);
		AudioSourceArray[(UINT)eBGMType::OPENING] = pAudioSource;
	}
	{
		AudioSource* pAudioSource = gameObject->AddComponent<AudioSource>();
		pAudioSource->SetVolume(0.3f);
		LoadBGM(pAudioSource, BGM_STAGE_TITLE);
		AudioSourceArray[(UINT)eBGMType::TITLE] = pAudioSource;
	}
	{
		AudioSource* pAudioSource = gameObject->AddComponent<AudioSource>();
		pAudioSource->SetVolume(0.3f);
		LoadBGM(pAudioSource, BGM_STAGE_01);
		AudioSourceArray[(UINT)eBGMType::STAGE_1] = pAudioSource;
	}
	{
		AudioSource* pAudioSource = gameObject->AddComponent<AudioSource>();
		pAudioSource->SetVolume(0.3f);
		LoadBGM(pAudioSource, BGM_STAGE_02);
		AudioSourceArray[(UINT)eBGMType::STAGE_2] = pAudioSource;
	}
	{
		AudioSource* pAudioSource = gameObject->AddComponent<AudioSource>();
		pAudioSource->SetVolume(0.3f);
		LoadBGM(pAudioSource, BGM_STAGE_BOSS);
		AudioSourceArray[(UINT)eBGMType::STAGE_BOSS] = pAudioSource;
	}
	{
		AudioSource* pAudioSource = gameObject->AddComponent<AudioSource>();
		pAudioSource->SetVolume(0.3f);
		LoadBGM(pAudioSource, BGM_STAGE_ENDING);
		AudioSourceArray[(UINT)eBGMType::ENDING] = pAudioSource;
	}
}

void BGMSelecter::SetBGM(eBGMType _type)
{
	if (mCurrAudio == AudioSourceArray[(UINT)_type])
		return;
	if (mCurrAudio)
	{
		if (mCurrAudio == AudioSourceArray[(int)_type])
			return;
		mCurrAudio->Reset();
	}
	if (_type != eBGMType::NONE)
	{
		mCurrAudio = AudioSourceArray[(int)_type];
		mCurrAudio->Play();
	}
}

void BGMSelecter::ChangeBGM(eBGMType _type)
{
	if (mCurrAudio == AudioSourceArray[(UINT)_type])
		return;
	if (mCurrAudio)
	{
		if (mCurrAudio == AudioSourceArray[(int)_type])
			return;
		mCurrAudio->FadeOut(1.0f);
	}
	if (_type != eBGMType::NONE)
	{
		mCurrAudio = AudioSourceArray[(int)_type];
		mCurrAudio->FadeIn(1.0f);
	}
}

void BGMSelecter::LoadBGM(AudioSource* _dstSource, const std::wstring& _filename)
{
	ResourceHandle ButtonSoundHandle;
	ButtonSoundHandle.mResourceType = eResourceType::AudioResource;
	ButtonSoundHandle.mMainKey = _filename;
	ButtonSoundHandle.mPath = L"resource/sound/" + _filename;
	if (ResourceManager::GetResource<AudioResource>(ButtonSoundHandle) == nullptr)
	{
		ResourceManager::LoadFileFromHandle(ButtonSoundHandle);
		ResourceManager::GetResource<AudioResource>(ButtonSoundHandle)->SetLoop(true);
	}
	_dstSource->AddAudio(_filename, ButtonSoundHandle);
}