#pragma once
#include "AudioSystem/Interface/IAudio.h"

class AudioGroup;

enum class eAudioListenMode
{
	DEFAULT,
	SOUND_2D,
	SOUND_3D,
};

class AudioClip : public IAudioResource
{
public:
	explicit AudioClip(std::wstring _wPath, bool _isLoop = true);
	virtual ~AudioClip();
public:
	virtual HRESULT Create() override;
	virtual void	Release() override;
public:
	// 오디오 그룹을 설정합니다.
	void SetGroup(AudioGroup* _pGroup);
	// 루프 설정을 합니다.
	void SetLoop(BOOL _isLoop);
	// 3D 입체 음향을 사용합니다. (왠만해선 사용X. 채널에서 설정 가능)
	void SetSoundMode(eAudioListenMode _soundMode);
public:
	std::wstring		mPath;
	AudioGroup*			mGroup;
	FMOD::Sound*		mSound;
public:
	inline auto GetSound() { return mSound; }
	inline auto GetGroup() { return mGroup; }
};

