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
	// ���� ������ �մϴ�.
	void SetLoop(bool _isLoop);
	// 3D ��ü ������ ����մϴ�.
	void SetSurround(bool _isSuround);
	// ������� ��ġ�� �������ݴϴ�. (3D ����)
	void SetPosition(Vector3 _position, Vector3 _velocity = { 0,0,0 });
private:
	bool isPaused;
	UINT mFrame;
	FMOD::Channel* mChannel;
public:
	inline FMOD::Channel* GetChannel() { return mChannel; }
};

