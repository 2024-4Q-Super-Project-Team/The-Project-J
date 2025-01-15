#pragma once
#include "AudioSystem/Interface/IAudio.h"

class AudioClip;
class AudilGroup;

class AudioChannel : public IAudioResource
{
public:
	explicit AudioChannel();
	virtual ~AudioChannel();
public:
	virtual HRESULT Create() override;
	virtual void	Release() override;
public:
	bool IsPlaying();
	bool IsPaused();
	HRESULT Play(AudioClip* _pAudio);
	HRESULT Reset();
	HRESULT Pause();
	HRESULT Resume();
public:
	// 루프 설정을 합니다.
	void SetLoop(bool _isLoop);
	// 3D 입체 음향을 사용합니다.
	void SetSurround(bool _isSuround);
private:
	bool isPaused;
	UINT mFrame;
	FMOD::Channel* mChannel;
};

