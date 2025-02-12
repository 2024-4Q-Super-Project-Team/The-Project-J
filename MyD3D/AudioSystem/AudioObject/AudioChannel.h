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
	// 오디오의 위치를 지정해줍니다. (3D 전용)
	void SetPosition(Vector3 _position, Vector3 _velocity = { 0,0,0 });
	// 오디오 볼륨 설정
	void SetVolume(FLOAT _val);
public:
	inline FLOAT GetVolume() { return mVolume; }
private:
	bool isPaused;
	UINT mFrame;
	FMOD::Channel* mChannel;
	FLOAT mVolume;
public:
	inline FMOD::Channel* GetChannel() { return mChannel; }
};

